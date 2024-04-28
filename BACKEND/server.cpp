#include "string"
#include "vector"
#include "unordered_set"

#include "crow.h"
#include "crow/middlewares/cors.h"

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/instance.hpp>

#include <cstdint>
#include <iostream>
#include <vector>

#include <boost/json/src.hpp>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

// #include "Module/Module.h"
#include "User/User.h"
#include "server.h"

struct Request {
    std::string request_id;
    std::string name;
    std::string project_owner;
    std::string assigned_manager;
    std::string status;
    std::string item_id;

    int quantity;
    int expense;

    std::string convertString() {
        std::string alpha = "{'request_id': '" + request_id + "', 'name': '" + name + "', 'project_owner': '" + project_owner + "', 'assigned_manager': '" + assigned_manager + "', 'status': '" + status + "', 'item_id': '" + item_id + ", 'quantity': " + std::to_string(int(quantity)) + ", 'expense': " + std::to_string(int(expense)) + "}|";
        return alpha;
    }
};

// void LiveHandlerModule(crow::App<crow::CORSHandler,UserMiddleware> *server, mongocxx::database *db_loc, std::unordered_set<crow::websocket::connection *> *request_list_users, std::unordered_set<crow::websocket::connection *> *inventory_list_users) {
//     crow::App<crow::CORSHandler,UserMiddleware> &app = *server;
//     mongocxx::database &db = *db_loc;

//     std::mutex mtx;
//     std::unordered_set<crow::websocket::connection *> &iusers = *inventory_list_users;
//     std::unordered_set<crow::websocket::connection *> &rusers = *request_list_users;

//         CROW_WEBSOCKET_ROUTE(app, "/api/list/view/live")
//       .onopen([&](crow::websocket::connection& conn) {
//           CROW_LOG_INFO << "new websocket connection from " << conn.get_remote_ip();
//           std::lock_guard<std::mutex> _(mtx);
//       })
//       .onclose([&](crow::websocket::connection& conn, const std::string& reason) {
//           CROW_LOG_INFO << "websocket connection closed: " << reason;
//           std::lock_guard<std::mutex> _(mtx);
//       })
//       .onmessage([&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary) {
//           std::lock_guard<std::mutex> _(mtx);
//       });
// }

void createRoutes(){

    // Module
    // Module _module;
    // _module.createRoutes();

    // User
    User user;
    user.createRoutes();
}

int main() {


    crow::App<crow::CORSHandler,VerifyUserMiddleware,LoginMiddleware> app;

    // Customize CORS
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors
      .global()
        .headers("X-Custom-Header", "Upgrade-Insecure-Requests")
        .methods("POST"_method, "GET"_method)
      .prefix("/")
        .origin("*");

    // Database
    mongocxx::instance inst{};
    const auto uri = mongocxx::uri{"mongodb+srv://manish:1234@cluster.d2khew0.mongodb.net/?retryWrites=true&w=majority"};

    mongocxx::client conn{uri};
    mongocxx::database db = conn["IMS"];

    Server* s = Server::getInstance();
    s->setServerData(&app,&db);

    // WS
    std::unordered_set<crow::websocket::connection *> inventory_list_users;
    std::unordered_set<crow::websocket::connection *> request_list_users;

    CROW_ROUTE(app, "/")
    ([db]() {
        return "<h1>IMS Status OK 4</h1>";
    });

    createRoutes();

    // LiveHandlerModule(&app, &db, &request_list_users, &inventory_list_users);

    app.bindaddr("127.0.0.1")
    .port(5000)
        .multithreaded()
        .run();
}

// To run:
/*
g++ --std=c++11 combined/server.cpp -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 -L/usr/local/lib -lmongocxx -lbsoncxx -lpthread -lboost_system -o app.out
*/

/*
Create a server.h while this file would be renamed to main.cpp 
so in server.h i would have a server class 
in server class i can have app, db, sessions, users
in main.cpp i would include User Inventory RequestManagement classes
while in all this files User Inventory RequestManagement i could include server.h so that i can get the value of app, db, .. in Inventory Request ..
*/