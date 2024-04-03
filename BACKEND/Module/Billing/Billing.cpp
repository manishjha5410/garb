#include "Billing.h"
#include "../../server.h"

Billing::Billing() {
    Server& s = Server::getInstance();
    app = s.app;
    db = s.db;
}

void Billing::BillingAdd() {

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/bill/add")
        .methods("POST"_method)([db_ref](const crow::request &req) {
            auto reqj = crow::json::load(req.body);
            if (!reqj)
                return crow::response(crow::status::BAD_REQUEST);

            mongocxx::collection collection = db_ref["billing"];

            auto builder = bsoncxx::builder::stream::document{};
            bsoncxx::document::value doc_value = builder
                                                 << "bill_id"
                                                 << reqj["bill_id"].s()
                                                 << "item_id"
                                                 << reqj["item_id"].s()
                                                 << "user_id"
                                                 << reqj["user_id"].s()
                                                 << "manager_id"
                                                 << reqj["manager_id"].s()
                                                 << "price"
                                                 << int(reqj["price"].i())
                                                 << "quantity"
                                                 << int(reqj["quantity"].i())
                                                 << bsoncxx::builder::stream::finalize;
            bsoncxx::document::view docview = doc_value.view();

            bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(docview);

            return crow::response(crow::status::OK);
        });
}

void Billing::BillingView() {

    mongocxx::database& db_ref = *db;

    CROW_ROUTE((*app), "/api/bill/view")
    ([db_ref]() {
        mongocxx::collection collection = db_ref["billing"];

        mongocxx::cursor cursor = collection.find({});

        std::string main_str = "[";

        crow::json::wvalue x;

        for (auto doc : cursor) {
            main_str += (bsoncxx::to_json(doc)) + ",";
        }
        // main_str.pop_back();

        main_str += "]";

        return main_str;
    });
}