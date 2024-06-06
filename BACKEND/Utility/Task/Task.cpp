#include "Task.h"
#include "../../Helper/helper.h"

Task::Task() {
    s = Server::getInstance();
    bp = new crow::Blueprint("task");
    s->app->register_blueprint(*bp);
    db = s->db;
}

crow::json::wvalue wTaskSchema = {
    {"_id", {
        {"insert", "No"},
        {"update", "No"}
    }},
    {"id", {
        {"type", "Integer"},
        {"skip", "Yes"},
        {"required", "No"},
        {"size", 9}
    }},
    {"assignAt", {
        {"type", "String"},
        {"required", "No"},
        {"skip", "Yes"},
    }},
    {"expireAt", {
        {"type", "String"},
        {"required", "No"},
        {"skip", "Yes"},
    }},
    {"priority", {
        {"type", "String"},
        {"value", {{"high", "medium","low"}}},
        {"required", "No"}
    }},
    {"server_id", {
        {"type", "String"},
        {"required", "No"},
        {"skip", "Yes"},
        {"size", 9}
    }},
    {"project_id", {
        {"type", "Integer"},
        {"skip", "Yes"},
        {"size", 9}
    }},
    {"quantity", {
        {"type", "Integer"}
    }}
};

auto TaskSchema = crow::json::load(wTaskSchema.dump());

void Task::createRoutes()
{
    TaskAdd();
}


void Task::TaskAdd()
{
    mongocxx::database &db_ref = *db;
    auto &app = s->app;

    CROW_BP_ROUTE((*bp), "/add")
    .CROW_MIDDLEWARES((*s->app), VerifyUserMiddleware)
    .methods(crow::HTTPMethod::Post)
    ([db_ref,app](const crow::request &req)
    {
        try
        {
            mongocxx::collection collection = db_ref["task"];

            crow::json::rvalue reqj = crow::json::load(req.body);

            if (!reqj)
                return crow::response(crow::status::BAD_REQUEST);
    
            std::pair<std::string, bool> check = JsonValid(reqj, TaskSchema); 

            if(!check.second)
                return crow::response(crow::status::BAD_REQUEST, check.first);

            auto& ctx = app->get_context<VerifyUserMiddleware>(req);
            std::string user_role = ctx.user_data["role"].as_string().c_str();
            std::string user_id = ctx.user_data["_id"].as_object()["$oid"].as_string().c_str();

            if(user_role != "employee")
                return crow::response(crow::status::UNAUTHORIZED, "User Should be employee");

            bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
            auto finalizer = bsoncxx::builder::stream::finalize;
            auto open = bsoncxx::builder::stream::open_document;
            auto close = bsoncxx::builder::stream::close_document;

            std::string input = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::_V2::system_clock::now().time_since_epoch()).count());
            int id = adler_hash(input);

            bsoncxx::document::value projection = builder << "_id" << 1 << "quantity" << 1 << "employee_id" << 1 << "priority" << 1 <<finalizer;
            bsoncxx::document::value filter = builder<<"id"<<reqj["project_id"].i()<<finalizer;

            bsoncxx::stdx::optional<bsoncxx::document::value> finder = db_ref["project"].find_one(filter.view(), mongocxx::options::find{}.projection(projection.view()));
            if(!finder)
                throw std::runtime_error("Unable to find Project");
            const bsoncxx::document::value& finder_str = *finder;

            if(reqj["quantity"].i() == 0)
                throw std::runtime_error("Enter Quantity");

            if(reqj["quantity"].i() > finder_str["quantity"].get_int32().value)
                throw std::runtime_error("Quantity should not be greater than project inventory");

            if(finder_str["employee_id"].get_oid().value.to_string() != user_id)
                throw std::runtime_error("User should be same as project employee");

            bsoncxx::builder::stream::document update_builder{};
            update_builder << "$inc" << open << "task_count" << 1 << "quantity" << -(reqj["quantity"].i()) << close;

            bsoncxx::document::value update = update_builder << finalizer;

            bsoncxx::stdx::optional<mongocxx::result::update> result_inven = db_ref["project"].update_one(filter.view(), update.view());
            if(!result_inven) 
                throw std::runtime_error("Error while Updating Project");

            bsoncxx::builder::stream::document insert_builder{};

            for (auto it = reqj.begin(); it != reqj.end(); ++it) {
                std::string key = std::string(it->key());
                if(TaskSchema[key].has("skip") && boost::iequals(TaskSchema[key]["skip"].s(),"Yes")) continue;
                auto var = convertData(*it);
                std::visit([&insert_builder,&it,&key](auto& value) { insert_builder<< key<< value; }, var);
            }

            std::string priority = finder_str["priority"].get_string().value.to_string();
            std::transform(priority.begin(), priority.end(), priority.begin(), ::tolower); 

            insert_builder<<"id"<<id;
            insert_builder<<"project_id"<<bsoncxx::oid(finder_str["_id"].get_oid().value.to_string());
            insert_builder<<"priority"<<priority;

            bsoncxx::document::value doc_value = insert_builder << finalizer;
            bsoncxx::document::view docview = doc_value.view();

            bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(docview);
            const mongocxx::result::insert_one& result_str = *result;

            if(!result) throw std::runtime_error("Error while insertion");

            std::ostringstream oss;
            oss << result_str.inserted_id().get_oid().value.to_string();

            crow::json::wvalue reqj_wvalue = crow::json::rvalue(reqj);
            reqj_wvalue["_id"] = oss.str();
            reqj_wvalue["id"] = id;
            reqj_wvalue["priority"] = priority;
            reqj_wvalue["project_id"] = finder_str["_id"].get_oid().value.to_string();

            std::string return_str = reqj_wvalue.dump();

            return crow::response(crow::status::CREATED, return_str);
        }
        catch (const std::exception& e) 
        {
            return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
        }
    });
}