#include "User.h"
#include "../Helper/helper.h"
#include "../Helper/md5.h"
#include "../server.h"

User::User() {
    Server& s = Server::getInstance();
    app = s.app;
    db = s.db;
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
        {"type", "String"}
    }},
    {"role", {
        {"type", "String"},
        {"value", {{"manager", "employee","admin"}}},
        {"update", "No"}
    }}
};

void User::createRoutes(){
    UserSignUp();
    UserDelete();
    UserViewOne();
    UserView();
    UserSignin();
}

void User::UserSignUp(){

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/signup")
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
                    if(count >= 0) return crow::response(crow::status::CONFLICT, "Admin is already present");
                }

                std::string input = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::_V2::system_clock::now().time_since_epoch()).count());
                int id = adler_hash(input);

                bsoncxx::builder::stream::document insert_builder{};

                for (auto it = reqj.begin(); it != reqj.end(); ++it) {
                    auto key = std::string(it->key());
                    auto var = convertData(*it);
                    std::visit([&insert_builder,&it,&key](auto& value) { insert_builder<< key<< value; }, var);
                }

                insert_builder<<"id"<<id;

                std::string hashedpassword = md5(std::string(reqj["password"].s()));
                insert_builder<< "password" << hashedpassword;

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

    CROW_ROUTE((*app), "/api/user/delete/<int>")
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

    CROW_ROUTE((*app), "/api/user/view")
    ([db_ref]() {
        try{
            mongocxx::collection collection = db_ref["user"];

            bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
            auto finalizer = bsoncxx::builder::stream::finalize;

            bsoncxx::document::value projection = builder << "_id" << 0 <<finalizer;
            mongocxx::cursor cursor = collection.find({},mongocxx::options::find{}.projection(projection.view()));

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

    CROW_ROUTE((*app), "/api/user/view/<int>")
        .methods("GET"_method)([db_ref](const int& id) {
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

    CROW_ROUTE((*app), "/api/login")
        .methods("POST"_method)([db_ref](const crow::request &req) {
            try {
                crow::json::rvalue reqj = crow::json::load(req.body);
                auto userSchema = crow::json::load(wUserSchema.dump());
                std::pair<std::string, bool> check = JsonValid(reqj, userSchema, 2); 

                if (!reqj)
                    return crow::response(crow::status::BAD_REQUEST);

                if(!check.second)
                    return crow::response(crow::status::BAD_REQUEST, check.first);

                mongocxx::collection collection = db_ref["user"];

                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                bsoncxx::document::value projection = builder << "email" << 1<<"password"<<1<<finalizer;
                bsoncxx::document::value filter = builder<<"email"<<std::string(reqj["email"].s())<<finalizer;

                bsoncxx::stdx::optional<bsoncxx::document::value> finder = collection.find_one(filter.view(), mongocxx::options::find{}.projection(projection.view()));
                if(!finder)
                    throw std::runtime_error("Unable to find document");

                const bsoncxx::document::value& finder_str = *finder;

                bsoncxx::document::view::const_iterator it = finder_str.find("password");
                std::string pass_hash = it->get_string().value.to_string();

                std::string hashed_now = md5(reqj["password"].s());

                std::cout<<"HASHED "<<hashed_now<<" "<<pass_hash<<std::endl;

                if(pass_hash != hashed_now)
                    return crow::response(crow::status::UNAUTHORIZED,"Incorrect Password");

                std::string json_str = bsoncxx::to_json(finder_str);
                return crow::response(crow::status::ACCEPTED,json_str);
            } catch (const std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
            }
        });
}