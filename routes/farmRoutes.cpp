#pragma once

#include <crow.h>

#include <iostream>
#include <string>

#include "jwt-cpp/jwt.h"
#include <dotenv/dotenvFind.h>

#include "../include/Encryption.h"
#include "../include/Database.h"
#include "../include/middleware/JWTMiddleware.h"

using namespace std;

inline void register_farmRoutes(crow::App<JWTMiddleware> &app) {

   CROW_ROUTE(app, "/farm")
   .methods("POST"_method)
   ([](const crow::request &req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            crow::json::wvalue res; res["error"] = "Invalid JSON";
            return crow::response(400, res);
        }

      );
}