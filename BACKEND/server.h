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

struct UserMiddleware: crow::ILocalMiddleware
{
    struct context
    {
        std::string username;
        std::string role;
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
        std::cout<<"Data is "<<req.url<<" "<<req.remote_ip_address<<" PORT is "<<req.get_header_value("Authorization")<<std::endl;
    }


    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {
    }
};

class Server {
public:
    crow::App<crow::CORSHandler,UserMiddleware> *app;
    mongocxx::database *db;

    struct User {
        std::string username;
        std::string role;
    };

    static Server& getInstance() {
        static Server instance;
        return instance;
    }

    void setServerData(crow::App<crow::CORSHandler,UserMiddleware> *server, mongocxx::database *db_loc) {
        app = server;
        db = db_loc;
    }

    private:
    Server() {};


};

#endif