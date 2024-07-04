#include "Billing.h"
#include "../Helper/helper.h"

Billing::Billing() {
    s = Server::getInstance();
    bp = new crow::Blueprint("billing");
    s->app->register_blueprint(*bp);
    db = s->db;
}

crow::json::wvalue wBillingSchema = {
    {"employee_id", {
        {"type", "Integer"},
        {"update", "No"},
        {"size", 9},
        {"required", "No"}
    }},
    {"project_id", {
        {"type", "Integer"},
        {"update", "No"},
        {"size", 9}
    }},
    {"to_paid", {
        {"type", "Integer"},
        {"required", "No"}
    }},
    {"to_get", {
        {"type", "Integer"},
        {"required", "No"}
    }}
};

void Billing::createRoutes()
{
    BillingView();
    BillingAdd();
}

void Billing::BillingAdd()
{
    mongocxx::database& db_ref = *db;
    auto &app = s->app;

    CROW_BP_ROUTE((*bp), "/add")
    .CROW_MIDDLEWARES((*s->app), VerifyUserMiddleware)
        .methods(crow::HTTPMethod::Post)([db_ref,app](const crow::request &req) {
            try {
                crow::json::rvalue reqj = crow::json::load(req.body);

                auto billingSchema = crow::json::load(wBillingSchema.dump());
                std::pair<std::string, bool> check = JsonValid(reqj, billingSchema); 

                if (!reqj)
                    return crow::response(crow::status::BAD_REQUEST);

                if(!check.second)
                    return crow::response(crow::status::BAD_REQUEST, check.first);

                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                auto& ctx = app->get_context<VerifyUserMiddleware>(req);
                std::string user_role = ctx.user_data["role"].as_string().c_str();
                std::string user_id = ctx.user_data["_id"].as_object()["$oid"].as_string().c_str();

                if(user_role!="manager")
                    throw std::runtime_error("User is not manager");

                bsoncxx::document::value projection = builder << "_id" << 1 << "quantity" << 1 <<"employee_id"<<1<<"expense"<<1<<"income_generated"<<1<<"manager_id"<<1 <<finalizer;
                bsoncxx::document::value filter = builder<<"id"<<reqj["project_id"].i()<<finalizer;

                bsoncxx::stdx::optional<bsoncxx::document::value> finder = db_ref["project"].find_one(filter.view(), mongocxx::options::find{}.projection(projection.view()));
                if(!finder)
                    throw std::runtime_error("Unable to find Project");
                const bsoncxx::document::value& finder_str = *finder;

                std::cout<<"ID is "<<user_id<<" "<<finder_str["manager_id"].get_oid().value.to_string()<<std::endl;
                // throw std::invalid_argument("Hello");

                if(finder_str["quantity"].get_int32().value == 0)
                    throw std::runtime_error("Project Quantity should be zero");

                if(user_id != finder_str["manager_id"].get_oid().value.to_string())
                    throw std::runtime_error("Project manager and logged in user is not same");

                mongocxx::collection collection = db_ref["billing"];
                std::string input = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::_V2::system_clock::now().time_since_epoch()).count());
                int id = adler_hash(input);

                bsoncxx::builder::stream::document insert_builder{};

                for (auto it = reqj.begin(); it != reqj.end(); ++it) {
                    std::string key = std::string(it->key());
                    if(billingSchema[key].has("skip") && boost::iequals(billingSchema[key]["skip"].s(),"Yes")) continue;
                    auto var = convertData(*it);
                    std::visit([&insert_builder,&it,&key](auto& value) { insert_builder<< key<< value; }, var);
                }

                insert_builder<<"project_id"<<bsoncxx::oid(finder_str["_id"].get_oid().value.to_string());
                insert_builder<<"employee_id"<<bsoncxx::oid(finder_str["employee_id"].get_oid().value.to_string());
                insert_builder<<"to_paid"<<finder_str["expense"].get_int32().value;
                insert_builder<<"to_get"<<finder_str["income_generated"].get_int32().value;
                insert_builder<<"id"<<id;

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
                reqj_wvalue["project_id"] = finder_str["_id"].get_oid().value.to_string();
                reqj_wvalue["employee_id"] = finder_str["employee_id"].get_oid().value.to_string();
                reqj_wvalue["to_paid"] = finder_str["expense"].get_int32().value;
                reqj_wvalue["to_get"] = finder_str["income_generated"].get_int32().value;

                std::string return_str = reqj_wvalue.dump();

                return crow::response(crow::status::CREATED, return_str);
            } catch (const std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
            }
        });
}

void Billing::BillingView() {

    mongocxx::database &db_ref = *db;
    auto &app = s->app;

    CROW_BP_ROUTE((*bp), "/view")
    .CROW_MIDDLEWARES((*s->app), VerifyUserMiddleware)
		.methods(crow::HTTPMethod::Get)
    ([db_ref,app](const crow::request& req) {
        try{
            mongocxx::collection collection = db_ref["billing"];

            auto& ctx = app->get_context<VerifyUserMiddleware>(req);
            std::string user_role = ctx.user_data["role"].as_string().c_str();

            bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
            auto finalizer = bsoncxx::builder::stream::finalize;
            auto open = bsoncxx::builder::stream::open_document;
            auto close = bsoncxx::builder::stream::close_document;

            bsoncxx::document::value projection = builder << "_id" << 0 <<finalizer;
            std::string user_id = ctx.user_data["_id"].as_object()["$oid"].as_string().c_str();


            bsoncxx::builder::basic::document filter = bsoncxx::builder::basic::document{};

            if (user_role == "employee") {
                filter.append(bsoncxx::builder::basic::kvp("manager_id", bsoncxx::oid{user_id}));
            }

            mongocxx::cursor cursor = collection.find(filter.view(), mongocxx::options::find{}.projection(projection.view()));

            std::string main_str = "[";

            for(mongocxx::cursor::iterator it = cursor.begin();it != cursor.end();){
                main_str += (bsoncxx::to_json(*it));
                if(std::next(it) != cursor.end()) main_str+=",";
            }

            main_str += "]";

            return crow::response(crow::status::OK, main_str);
        }
        catch (const std::exception& e) {
            return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
        } });
}