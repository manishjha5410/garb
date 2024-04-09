#ifndef SERVER_H_
#define SERVER_H_

#include "string"
#include "vector"
#include "unordered_set"

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "unordered_set"
#include "vector"
#include "Helper/Authorization.h"

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
    }


    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {
        // std::cout<<"Data is "<<req.url<<" "<<req.remote_ip_address<<" PORT is "<<res.body<<std::endl;
        boost::json::object data = boost::json::parse(res.body).as_object();
        Session& s = Session::getInstance();
        std::string email = data["email"].as_string().data();
        std::string password = data["password"].as_string().data();
        std::string jwt_str = "";
        std::pair<bool,std::string>ans = s.create_session(jwt_str,email,password);

        std::cout<<"Now is "<<ans.first<<" "<<ans.second<<std::endl;
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