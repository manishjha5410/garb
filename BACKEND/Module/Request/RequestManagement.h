#ifndef REQUEST_MANAGE_H_
#define REQUEST_MANAGE_H_

#include "crow.h"
#include "crow/middlewares/cors.h"

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_document;

class RequestManagement {
    public:
        crow::App<crow::CORSHandler> *app;
        mongocxx::database *db;

        int price;
        int quantity;

        RequestManagement();
        void RequestManagementAdd();
        void RequestManagementView();
        void RequestManagementAccept();
        void RequestManagementReject();
        void RequestManagementDelete();
};

#endif