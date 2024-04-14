#ifndef MIDDLEWARE_USER_H
#define MIDDLEWARE_USER_H

#include "chrono"
#include "sstream"

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "../Helper/Authorization.h"

struct VerifyUserMiddleware: crow::ILocalMiddleware
{
    struct context
    {
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
        Session& s = Session::getInstance();

        try
        {
            if(req.body=="") throw std::invalid_argument("Enter Payload");

            std::string jwtToken = req.get_header_value("Authorization");

            if(jwtToken=="") throw std::invalid_argument("Enter Authorization Header");

            std::pair<bool,std::string>ans = s.verify_session(jwtToken);

            if(!ans.first) throw std::runtime_error(ans.second);
        }
        catch (const std::exception& e) {
            res.code = 403;
            res.body = "";
            res.end(e.what());
        }
    }


    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {
    }
};

struct LoginMiddleware: crow::ILocalMiddleware
{
    struct context
    {
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {
        if(res.code >= 400) return;
std::cout<<"Data is \n";
        Session& s = Session::getInstance();

        try
        {
            boost::json::object data = boost::json::parse(res.body).as_object();

            std::stringstream ss;
            data.erase("_id");
            data.erase("name");

            std::string jwt_str = "";

            std::pair<bool,std::string>ans = s.create_session(data,jwt_str);

            data = boost::json::parse(res.body).as_object();

            if(!ans.first)
                throw std::runtime_error(ans.second);

            data["jwt"] = ans.second;

            ss << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - std::chrono::system_clock::from_time_t(0)).count();
            data["session_created"] = ss.str();
            ss.str("");
            ss<<data;
            res.body = ss.str();
        }
        catch (const std::exception& e) {
            res.code = crow::status::FORBIDDEN;
            res.body = "";
            res.end(e.what());
        }
    }
};

#endif