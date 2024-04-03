#ifndef MODULE_H_
#define MODULE_H_

#include "crow.h"
#include "crow/middlewares/cors.h"

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include "Billing/Billing.h"
#include "Request/RequestManagement.h"
#include "Inventory/Inventory.h"

class Module
{
    public:
        void createRoutes(){
            // Billing
            Billing billing;
            billing.BillingView();
            billing.BillingAdd();

            // Request
            RequestManagement request;
            request.RequestManagementAdd();
            request.RequestManagementView();
            request.RequestManagementAccept();
            request.RequestManagementReject();
            request.RequestManagementDelete();

            // Inventory
            Inventory inventory;
            inventory.InventoryAdd();
            inventory.InventoryEdit();
            inventory.InventoryDelete();
            inventory.InventoryViewOne();
            inventory.InventoryView();
            inventory.InventoryAssingServer();
        }
};

#endif