#include "Admin.h"
#include "../Helper/helper.h"

Admin::Admin() {
    s = Server::getInstance();
    bp = new crow::Blueprint("admin");
    s->app->register_blueprint(*bp);
    db = s->db;
}

Admin::createRoutes() {

}

Admin::UserView() {

}

Admin::UserViewOne() {
    
}