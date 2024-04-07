#include "RequestManagement.h"

RequestManagement::RequestManagement() {
    Server& s = Server::getInstance();
    app = s.app;
    db = s.db;
}

void RequestManagement::RequestManagementAdd() {
    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/request/add")
        .methods("POST"_method)([db_ref](const crow::request &req) {
            auto reqj = crow::json::load(req.body);
            if (!reqj)
                return crow::response(crow::status::BAD_REQUEST);
            mongocxx::collection collection = db_ref["request"];

            auto builder = bsoncxx::builder::stream::document{};
            bsoncxx::document::value doc_value = builder
                                                 << "request_id"
                                                 << reqj["request_id"].s()
                                                 << "name"
                                                 << reqj["name"].s()
                                                 << "project_owner"
                                                 << reqj["project_owner"].s()
                                                 << "assigned_manager"
                                                 << reqj["assigned_manager"].s()
                                                 << "status"
                                                 << "Pending"
                                                 << "item_id"
                                                 << reqj["item_id"].s()
                                                 << "quantity"
                                                 << int(reqj["quantity"].i())
                                                 << "expense"
                                                 << int(reqj["expense"].i())
                                                 << bsoncxx::builder::stream::finalize;
            bsoncxx::document::view docview = doc_value.view();

            bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(docview);

            return crow::response(crow::status::OK);
        });
}

void RequestManagement::RequestManagementView() {
    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/request/view")
        .methods("GET"_method)([db_ref]() {
            mongocxx::collection collection = db_ref["request"];

            mongocxx::cursor cursor = collection.find({});

            std::string main_str = "[";

            crow::json::wvalue x;

            for (auto doc : cursor) {
                main_str += (bsoncxx::to_json(doc)) + ",";
            }
            main_str.pop_back();

            main_str += "]";

            return main_str;
        });
}

void RequestManagement::RequestManagementAccept() {

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/request/accept")
        .methods("POST"_method)([db_ref](const crow::request &req) {
            auto reqj = crow::json::load(req.body);
            if (!reqj)
                return crow::response(crow::status::BAD_REQUEST);

            mongocxx::collection collection = db_ref["request"];

            bsoncxx::builder::stream::document query{};

            collection.update_one(query << "request_id" << reqj["request_id"].s() << bsoncxx::builder::stream::finalize,
                                  query << "$set" << open_document << "status"
                                        << "Pass" << close_document << bsoncxx::builder::stream::finalize);

            // for (auto &x : requests_db) {
            //     if (x.item_id == reqj["request_id"].s()) {
            //         x.status = "Pass";
            //     }
            // }

            return crow::response(crow::status::OK);
        });
}

void RequestManagement::RequestManagementReject() {

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/request/reject")
        .methods("POST"_method)([db_ref](const crow::request &req) {
            auto reqj = crow::json::load(req.body);
            if (!reqj)
                return crow::response(crow::status::BAD_REQUEST);

            mongocxx::collection collection = db_ref["request"];

            bsoncxx::builder::stream::document query{};

            collection.update_one(query << "request_id" << reqj["request_id"].s() << bsoncxx::builder::stream::finalize,
                                  query << "$set" << open_document << "status"
                                        << "Fail" << close_document << bsoncxx::builder::stream::finalize);

            // for (auto &x : requests_db) {
            //     if (x.item_id == reqj["request_id"].s()) {
            //         x.status = "Failed";
            //     }
            // }

            return crow::response(crow::status::OK);
        });
}

void RequestManagement::RequestManagementDelete() {

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/request/delete")
        .methods("POST"_method)([db_ref](const crow::request &req) {
            auto reqj = crow::json::load(req.body);
            if (!reqj)
                return crow::response(crow::status::BAD_REQUEST);

            mongocxx::collection collection = db_ref["request"];

            auto builder = bsoncxx::builder::stream::document{};

            bsoncxx::stdx::optional<mongocxx::result::delete_result> result = collection.delete_one(builder << "request_id" << reqj["id"].s() << bsoncxx::builder::stream::finalize);

            if (result) {
                std::cout << result->deleted_count() << "\n";
            }

            return crow::response(crow::status::OK);
        });
}