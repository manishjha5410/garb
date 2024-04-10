#ifndef SERVER_H_
#define SERVER_H_

#include "string"
#include "vector"
#include "chrono"
#include "unordered_set"
#include "sstream"

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
        if((req.method == crow::HTTPMethod::PUT || req.method == crow::HTTPMethod::POST) && req.body=="")
        {
            res.code = 403;
            res.end("Enter Payload");
            return;
        }
    }


    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {
        if(res.code == 403) return;

        boost::json::object data = boost::json::parse(res.body).as_object();
        Session& s = Session::getInstance();

        std::stringstream ss;

        std::string email = data["email"].as_string().data();
        std::string password = data["password"].as_string().data();
        std::string jwt_str = "";


        std::pair<bool,std::string>ans = s.create_session(jwt_str,email,password);

        if(!ans.first) {
            res.code = crow::status::FORBIDDEN;
            res.body = ans.second;
            return;
        }

        data["jwt"] = ans.second;

        ss << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - std::chrono::system_clock::from_time_t(0)).count();
        data["session_created"] = ss.str();
        ss.str("");
        ss<<data;
        res.body = ss.str();
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