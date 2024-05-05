#ifndef UTILITY_H_
#define UTILITY_H_

#include "crow.h"
#include "crow/middlewares/cors.h"

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include "Project/Project.h"

class Utility
{
    public:
        void createRoutes(){
            // Project
            Project project;
            project.createRoutes();
        }
};

#endif