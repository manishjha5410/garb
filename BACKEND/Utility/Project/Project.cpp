#include "Project.h"
#include "../Helper/helper.h"

Project::Project() {
    s = Server::getInstance();
    bp = new crow::Blueprint("project");
    s->app->register_blueprint(*bp);
    db = s->db;
}

crow::json::wvalue wProjectSchema = {
    {"_id", {
        {"insert", "No"},
        {"update", "No"}
    }},
    {"id", {
        {"insert", "No"},
        {"update", "No"}
    }},
    {"name", {
        {"type", "String"}
    }},
    {"type", {
        {"type", "String"}
    }},
    {"quantity", {
        {"type", "Integer"}
    }},
    {"income", {
        {"type", "Double"}
    }},
    {"expected_time", {
        {"type", "Integer"},
        {"required", "No"}
    }},
    {"employee_id", {
        {"type", "String"},
        {"required", "No"},
        {"skip", "Yes"},
    }},
    {"request_id", {
        {"type", "List"},
        {"required", "No"},
        {"skip", "Yes"},
    }},
    {"billing_id", {
        {"type", "String"},
        {"required", "No"},
        {"skip", "Yes"},
    }},
    {"priority", {
        {"type", "String"},
        {"value", {{"high", "medium","low"}}}
    }},
    {"status", {
        {"type", "String"},
        {"value", {{"stale", "running","completed"}}}
    }},
    {"manager_id", {
        {"type", "String"},
        {"required", "No"},
        {"skip", "Yes"},
    }},
};


void Project::createRoutes() {
    ProjectAdd();
}

void Project::ProjectAdd() {
    mongocxx::database &db_ref = *db;
    auto &app = s->app;

    CROW_BP_ROUTE((*bp), "/add")
    .CROW_MIDDLEWARES((*s->app), VerifyUserMiddleware)
    .methods(crow::HTTPMethod::Post)
    ([db_ref,app](const crow::request &req)
    {
        try
        {
            mongocxx::collection collection = db_ref["project"];

            crow::json::rvalue reqj = crow::json::load(req.body);

            if (!reqj)
                return crow::response(crow::status::BAD_REQUEST);

            auto projectSchema = crow::json::load(wProjectSchema.dump());
            std::pair<std::string, bool> check = JsonValid(reqj, projectSchema); 

            if(!check.second)
                return crow::response(crow::status::BAD_REQUEST, check.first);

            auto& ctx = app->get_context<VerifyUserMiddleware>(req);
            std::string user_role = ctx.user_data["role"].as_string().c_str();
            std::string user_id = ctx.user_data["_id"].as_object()["$oid"].as_string().c_str();

            if(user_role != "manager")
                return crow::response(crow::status::UNAUTHORIZED, "User Should be manager");

            bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
            auto finalizer = bsoncxx::builder::stream::finalize;
            auto open = bsoncxx::builder::stream::open_document;
            auto close = bsoncxx::builder::stream::close_document;

            std::string input = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::_V2::system_clock::now().time_since_epoch()).count());
            int id = adler_hash(input);

            bsoncxx::builder::stream::document insert_builder{};

            for (auto it = reqj.begin(); it != reqj.end(); ++it) {
                std::string key = std::string(it->key());
                if(projectSchema[key].has("skip") && boost::iequals(projectSchema[key]["skip"].s(),"Yes")) continue;
                auto var = convertData(*it);
                std::visit([&insert_builder,&it,&key](auto& value) { insert_builder<< key<< value; }, var);
            }

            insert_builder<<"id"<<id;
            insert_builder<<"manager_id"<<bsoncxx::oid(user_id);

            std::string json_str = bsoncxx::to_json(insert_builder);

            bsoncxx::document::value doc_value = insert_builder << finalizer;
            bsoncxx::document::view docview = doc_value.view();

            bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(docview);
            const mongocxx::result::insert_one& result_str = *result;

            if(!result) throw std::runtime_error("Error while insertion");

            std::ostringstream oss;
            oss << result_str.inserted_id().get_oid().value.to_string();

            crow::json::wvalue reqj_wvalue = crow::json::rvalue(reqj);
            reqj_wvalue["_id"] = oss.str();
            reqj_wvalue["manager_id"] = user_id;
            reqj_wvalue["id"] = id;

            std::string return_str = reqj_wvalue.dump();

            return crow::response(crow::status::CREATED, return_str);
        }
        catch (const std::exception& e) 
        {
            return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
        }
    });
}