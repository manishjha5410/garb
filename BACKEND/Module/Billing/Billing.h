#ifndef BILLING_H_
#define BILLING_H_

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "../../server.h"
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

class Billing {
    public:
        crow::Blueprint *bp;
        mongocxx::database *db;
        Server* s;

        int price;
        int quantity;

        std::string convertString();
        Billing();
        void BillingAdd();
        void BillingView();
};

#endif