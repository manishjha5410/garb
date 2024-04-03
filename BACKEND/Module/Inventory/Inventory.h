#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "unordered_set"
#include "vector"

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

class Inventory {
    public:
        crow::App<crow::CORSHandler> *app;
        mongocxx::database *db;

        Inventory();
        void InventoryAdd();
        void InventoryEdit();
        void InventoryDelete();
        void InventoryView();
        void InventoryViewOne();
        void InventoryAssingServer();
};

#endif