#include "Inventory.h"
#include "../../Helper/helper.h"

Inventory::Inventory() {
    Server& s = Server::getInstance();
    app = s.app;
    db = s.db;
}

crow::json::wvalue wInventorySchema = {
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
    {"priority", {
        {"type", "String"},
        {"value", {{"high", "medium","low"}}}
    }},
    {"type", {
        {"type", "String"}
    }},
    {"cost", {
        {"type", "Double"}
    }},
    {"available", {
        {"type", "Boolean"}
    }},
    {"life", {
        {"type", "Integer"}
    }},
    {"quantity", {
        {"type", "Integer"}
    }},
    {"task_id", {
        {"type", "List"},
        {"required", "No"}
    }},
};

auto inventorySchema = crow::json::load(wInventorySchema.dump());

void Inventory::InventoryAdd(){

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/list/add")
        .methods("POST"_method)([db_ref](const crow::request &req) {
            try {
                crow::json::rvalue reqj = crow::json::load(req.body);

                std::pair<std::string, bool> check = JsonValid(reqj, inventorySchema); 

                if (!reqj)
                    return crow::response(crow::status::BAD_REQUEST);

                if(!check.second)
                    return crow::response(crow::status::BAD_REQUEST, check.first);

                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                mongocxx::collection collection = db_ref["inventory"];
                bsoncxx::document::value count_view = builder << finalizer;
                std::string input = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::_V2::system_clock::now().time_since_epoch()).count());
                int id = adler_hash(input);

                std::string priority = reqj["priority"].s();
                std::transform(priority.begin(), priority.end(), priority.begin(), ::tolower);

                bsoncxx::builder::stream::document insert_builder{};

                for (auto it = reqj.begin(); it != reqj.end(); ++it) {
                    auto key = std::string(it->key());
                    auto var = convertData(*it);
                    std::visit([&insert_builder,&it,&key](auto& value) { insert_builder<< key<< value; }, var);
                }

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
                std::string return_str = reqj_wvalue.dump();

                return crow::response(crow::status::CREATED, return_str);
            } catch (const std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR, e.what());
            }
        });
}

void Inventory::InventoryEdit(){

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/list/edit/<int>")
        .methods("PUT"_method)([db_ref](const crow::request &req,const int& id) {
            crow::json::rvalue reqj = crow::json::load(req.body);

            if (!reqj)
                return crow::response(crow::status::BAD_REQUEST);

            std::pair<std::string, bool> check = JsonValid(reqj, inventorySchema, 1); 

            if(!check.second)
                return crow::response(crow::status::BAD_REQUEST, check.first);

            bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
            auto open = bsoncxx::builder::stream::open_document;
            auto close = bsoncxx::builder::stream::close_document;0
            auto finalizer = bsoncxx::builder::stream::finalize;

            bsoncxx::document::value filter = builder<<"id"<<id<<finalizer;

            bsoncxx::builder::stream::document update_builder{};

            update_builder<<"$set"<<open;
            for (auto it = reqj.begin(); it != reqj.end(); ++it) {
                auto key = std::string(it->key());
                auto var = convertData(*it);
                std::visit([&update_builder,&it,&key](auto& value) { update_builder<< key<< value; }, var);
            }
            update_builder<<close;

            bsoncxx::document::value update = update_builder << finalizer;        

            std::string json_str1 = bsoncxx::to_json(update);

            mongocxx::collection collection = db_ref["inventory"];

            bsoncxx::stdx::optional<mongocxx::result::update> result = collection.update_one(filter.view(), update.view());
            if(!result) 
                throw std::runtime_error("Error while Updation");
            bsoncxx::document::value projection = builder << "_id" << 0 <<finalizer;

            bsoncxx::stdx::optional<bsoncxx::document::value> finder = collection.find_one(filter.view(), mongocxx::options::find{}.projection(projection.view()));
            if(!finder)
                throw std::runtime_error("Updation done unable to find modified document");

            const bsoncxx::document::value& finder_str = *finder;
            std::string json_str = bsoncxx::to_json(finder_str);

            return crow::response(crow::status::ACCEPTED,json_str);
        });
}


void Inventory::InventoryDelete(){

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/list/delete/<int>")
        .methods("DELETE"_method)([db_ref](const int& id) {
            try{
                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                bsoncxx::document::value filter = builder<<"id"<<id<<finalizer;

                mongocxx::collection collection = db_ref["inventory"];
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

void Inventory::InventoryView(){

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/list/view")
    ([db_ref]() {
        try{
            mongocxx::collection collection = db_ref["inventory"];

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

void Inventory::InventoryViewOne(){

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/list/view/<int>")
        .methods("GET"_method)([db_ref](const int& id) {
            try {

                bsoncxx::builder::stream::document builder = bsoncxx::builder::stream::document{};
                auto finalizer = bsoncxx::builder::stream::finalize;

                bsoncxx::document::value projection = builder << "_id" << 0 <<finalizer;
                bsoncxx::document::value filter = builder<<"id"<<id<<finalizer;

                mongocxx::collection collection = db_ref["inventory"];
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

void Inventory::InventoryAssingServer(){

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/list/assign_server")
    ([db_ref]() {
        std::string main_str;

        mongocxx::collection collection = db_ref["inventory"];

        mongocxx::cursor cursor = collection.find({});

        return main_str;
    });
}