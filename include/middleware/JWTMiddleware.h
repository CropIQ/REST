#pragma once

#include <crow.h>
#include <jwt-cpp/jwt.h>
#include <dotenv/dotenvFind.h>

#include <chrono>
#include <string>
#include <vector>

using namespace std;

struct JWTMiddleware {
    struct context {
        bool isAuthenticated = false;
        string userRole_ = "";
        map<string, string> userData_;
        unique_ptr<crow::response> hasAnyRole(const vector<string>& requiredRoles);
        string getUserRole() const;
        map<string, string> getUserData() const;
    };

    void after_handle(crow::request& req, crow::response& res, context& ctx);
    void before_handle(crow::request& req, crow::response& res, context& ctx);
};
