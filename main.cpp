#include <crow.h>
#include <crow/middlewares/cors.h>

#include <iostream>
#include <string>

#include <dotenv/dotenvFind.h>
#include "jwt-cpp/jwt.h"

#include "include/Encryption.h"
#include "include/Database.h"
#include "include/middleware/JWTMiddleware.h"
#include "include/Routes.h"

using namespace std;

int main()
{
    crow::App<crow::CORSHandler, JWTMiddleware> app;
    dotenvFind::init();
    
    app.get_middleware<crow::CORSHandler>().global()
        .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method, "OPTIONS"_method)
        .headers("Content-Type, Authorization")
        .allow_credentials();

    // Use std::getenv("DATABASE_HOST") to get the environment variable

    register_authRoutes(app);
    register_workerRoutes(app);
    register_farmRoutes(app);

    /*
    ---------- SAMPLE AUTHENTICATION ROUTE ----------
    CROW_ROUTE(app, "/simpleAuth")
    .methods("GET"_method)
    ([&app](const crow::request& req){
        auto &ctx = app.get_context<JWTMiddleware>(req);
        if (auto error = ctx.hasAnyRole({"admin", "user"})) return std::move(*error);

        crow::json::wvalue x;
        x["message"] = "Hello " + ctx.getUserRole();

        for (const auto& kv : ctx.getUserData()) {
            x[kv.first] = kv.second;
        }

        return crow::response{200, x};
    });
    */
    
    /*
    ---------- SAMPLE DATABASE ROUTE ----------
    CROW_ROUTE(app, "/sampleDB")([](){
        Database db;
        if (!db.connect()) return crow::response(500, "Unexpected error");
        auto conn = db.getConn();
        
        auto stmt = db.createStatement();
        auto res = stmt->executeQuery("SELECT * FROM users");

        crow::json::wvalue users;
        vector<crow::json::wvalue> usersList;

        while (res->next()) {
            int id = res->getInt("id");
            string name = res->getString("name").c_str();
            usersList.push_back({{"id", id}, {"name", name}});
        }

        users["users"] = move(usersList);
        return crow::response{200, users};
    });
    */

    app.port(18080).multithreaded().run();
}