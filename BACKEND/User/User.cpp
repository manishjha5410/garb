#include "User.h"
#include "../Helper/helper.h"
#include "../Helper/md5.h"
#include <boost/algorithm/string/predicate.hpp>
#include <variant>


User::User() {
    s = Server::getInstance();
    bp = new crow::Blueprint("user");
    s->app->register_blueprint(*bp);
    db = s->db;
}

crow::json::wvalue wUserSchema = {
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
    {"email", {
        {"type", "String"},
        {"update", "No"},
        {"regex", "^(([^<>()[\\]\\.,;:\\s@\"]+(\\.[^<>()[\\]\\.,;:\\s@\"]+)*)|(\".+\"))@(([^<>()[\\]\\.,;:\\s@\"]+\\.)+[^<>()[\\]\\.,;:\\s@\"]{2,})$"}
    }},
    {"password", {
        {"type", "String"},
        {"skip", "Yes"},
    }},
    {"role", {
        {"type", "String"},
        {"value", {{"manager", "employee","admin"}}},
        {"update", "No"}
    }}
};

void User::createRoutes(){
    UserSignin();
    UserSignUp();
    UserDelete();
    UserViewOne();
    UserView();
}

void User::UserSignUp(){

    mongocxx::database& db_ref = *db;

    CROW_BP_ROUTE((*bp), "/signup")
        .methods("POST"_method)([db_ref](const crow::request &req) {
            try {
                crow::json::rvalue reqj = crow::json::load(req.body);
                auto userSchema = crow::json::load(wUserSchema.dump());
                std::pair<std::string, bool> check = JsonValid(reqj, userSchema); 

                if (!reqj)
                    return crow::response(crow::status::BAD_REQUEST);

                if(!check.second)
                    return crow::response(crow::status::BAD_REQUEST, check.first);

                mongocxx::collection collection = db_ref["user"];

                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                if(reqj["role"].s() == "admin")
                {
                    bsoncxx::document::value admin_present = builder<<"role"<<"admin"<<finalizer;
                    std::int64_t count = collection.count_documents(admin_present.view());
                    if(count > 0) return crow::response(crow::status::CONFLICT, "Admin is already present");
                }

                std::string input = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::_V2::system_clock::now().time_since_epoch()).count());
                int id = adler_hash(input);

                bsoncxx::builder::stream::document insert_builder{};


                for (auto it = reqj.begin(); it != reqj.end(); ++it) {
                    std::string key = std::string(it->key());
                    if(userSchema[key].has("skip") && boost::iequals(userSchema[key]["skip"].s(),"Yes")) continue;
                    auto var = convertData(*it);
                    std::visit([&insert_builder,&it,&key](auto& value) { insert_builder<< key<< value; }, var);
                }

                insert_builder<<"id"<<id;

                std::string hashedpassword = md5(std::string(reqj["password"].s()));
                insert_builder<< "password" << hashedpassword;

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
                reqj_wvalue["id"] = id;
                reqj_wvalue["password"] = hashedpassword;
                std::string return_str = reqj_wvalue.dump();

                return crow::response(crow::status::CREATED, return_str);
            } catch (const std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
            }
        });
}

void User::UserDelete(){

    mongocxx::database& db_ref = *db;

    CROW_BP_ROUTE((*bp), "/delete/<int>")
        .methods("DELETE"_method)([db_ref](const int& id) {
            try{
                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                bsoncxx::document::value filter = builder<<"id"<<id<<finalizer;

                mongocxx::collection collection = db_ref["user"];
                bsoncxx::stdx::optional<mongocxx::result::delete_result> deleter = collection.delete_one(filter.view());
                if(!deleter)
                    throw std::runtime_error("Unable to delete document");

                const mongocxx::result::delete_result& deleter_str = *deleter;

                std::string message = deleter_str.deleted_count() == 0 ? "No document to delete":"Document deleted sucessfully";
                crow::status status = deleter_str.deleted_count() == 0 ? crow::status::BAD_REQUEST : crow::status::ACCEPTED;

                return crow::response(status,message);
            }
            catch (const std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
            }
        });
}

void User::UserView(){

    mongocxx::database& db_ref = *db;
    auto &app = s->app;

    CROW_BP_ROUTE((*bp), "/view")
    .CROW_MIDDLEWARES((*s->app), VerifyUserMiddleware)
		.methods(crow::HTTPMethod::Get)
    ([db_ref,app](const crow::request& req) {
        try{
            mongocxx::collection collection = db_ref["user"];

            auto& ctx = app->get_context<VerifyUserMiddleware>(req);
            std::string user_role = ctx.user_data["role"].as_string().c_str();

            bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
            auto finalizer = bsoncxx::builder::stream::finalize;
            auto open = bsoncxx::builder::stream::open_document;
            auto close = bsoncxx::builder::stream::close_document;

            bsoncxx::document::value projection = builder << "_id" << 0 <<finalizer;
            // bsoncxx::document::value filter = builder << "_id" << 0 <<finalizer;

            bsoncxx::builder::stream::document filter_builder{};

        filter_builder << "$or" << bsoncxx::builder::stream::open_array
                       << bsoncxx::builder::stream::open_document << "email" << std::string(ctx.user_data["email"].as_string().c_str()) << bsoncxx::builder::stream::close_document
                       << bsoncxx::builder::stream::open_document << "role" << "employee" << bsoncxx::builder::stream::close_document
                       << bsoncxx::builder::stream::close_array;


            filter_builder << "$or";
            filter_builder << "$or" << bsoncxx::builder::stream::open_array << bsoncxx::builder::stream::open_document << "email" << std::string(ctx.user_data["email"].as_string().c_str()) << bsoncxx::builder::stream::close_document;
            filter_builder << bsoncxx::builder::stream::close_array;
            filter_builder << "email" << std::string(ctx.user_data["email"].as_string().c_str());

            // std::cout<<user_role<<std::endl;

            // if(user_role == "manager"){
            //     filter_builder << "role" << "employee";
            // } else if (user_role == "employee"){
            //     filter_builder << "password" << std::string(ctx.user_data["password"].as_string().c_str());
            // } else {
            //     filter_builder << "name" << std::atoi(ctx.user_data["name"].as_string().c_str());
            // }

            // filter_builder << close;
            // filter_builder << finalizer;

            std::cout<<bsoncxx::to_json(filter_builder.view())<<std::endl;

            mongocxx::cursor cursor = collection.find(filter_builder.view(),mongocxx::options::find{}.projection(projection.view()));

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
        }
    });
}

void User::UserViewOne(){

    mongocxx::database& db_ref = *db;

    CROW_BP_ROUTE((*bp), "/view/<int>")
		.methods(crow::HTTPMethod::Get)
        ([db_ref](const int& id) {
            try {

                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                bsoncxx::document::value projection = builder << "_id" << 0 <<finalizer;
                bsoncxx::document::value filter = builder<<"id"<<id<<finalizer;

                mongocxx::collection collection = db_ref["user"];
                bsoncxx::stdx::optional<bsoncxx::document::value> finder = collection.find_one(filter.view(), mongocxx::options::find{}.projection(projection.view()));
                if(!finder)
                    throw std::runtime_error("Unable to find document");

                const bsoncxx::document::value& finder_str = *finder;
                std::string json_str = bsoncxx::to_json(finder_str);

                return crow::response(crow::status::OK,json_str);
            } catch (const std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
            }
        });
}


void User::UserSignin(){

    mongocxx::database& db_ref = *db;
    // UserMiddleware usermid = Server::getInstance().middleware;

    CROW_BP_ROUTE((*bp), "/login")
    .CROW_MIDDLEWARES((*s->app), LoginMiddleware)
		.methods(crow::HTTPMethod::Post)
        ([db_ref](const crow::request &req) {
            try {

                crow::json::rvalue reqj = crow::json::load(req.body);

                auto userSchema = crow::json::load(wUserSchema.dump());


                std::pair<std::string, bool> check = JsonValid(reqj, userSchema, 2); 
                if(!check.second)
                    return crow::response(crow::status::BAD_REQUEST, check.first);

                mongocxx::collection collection = db_ref["user"];

                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                bsoncxx::document::value filter = builder<<"email"<<std::string(reqj["email"].s())<<finalizer;

                bsoncxx::stdx::optional<bsoncxx::document::value> finder = collection.find_one(filter.view());
                if(!finder)
                    throw std::runtime_error("Unable to find document");

                const bsoncxx::document::value& finder_str = *finder;

                bsoncxx::document::view::const_iterator it = finder_str.find("password");
                std::string pass_hash = it->get_string().value.to_string();

                std::string hashed_now = md5(reqj["password"].s());
                std::string json_str = bsoncxx::to_json(finder_str);

                if(pass_hash != hashed_now)
                    return crow::response(crow::status::UNAUTHORIZED,"Incorrect Password");

                return crow::response(crow::status::ACCEPTED,json_str);
            } catch (const std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
            }
        });
}