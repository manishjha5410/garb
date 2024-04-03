#ifndef SERVER_H_
#define SERVER_H_

#include "string"
#include "vector"
#include "unordered_set"

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "unordered_set"
#include "vector"

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

class Server {
public:
    crow::App<crow::CORSHandler> *app;
    mongocxx::database *db;

    struct User {
        std::string username;
        std::string role;
    };

    static Server& getInstance() {
        static Server instance;
        return instance;
    }

    void setServerData(crow::App<crow::CORSHandler> *server, mongocxx::database *db_loc) {
        app = server;
        db = db_loc;
    }

    private:
    Server() {};


};

#endif