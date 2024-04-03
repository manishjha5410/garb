#ifndef USER_H_
#define USER_H_

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


class User {
    public:
        crow::App<crow::CORSHandler> *app;
        mongocxx::database *db;

        User();
        void createRoutes();
        void UserSignUp();
        void UserEdit();
        void UserDelete();
        void UserView();
        void UserViewOne();
        void UserSignin();
};

#endif