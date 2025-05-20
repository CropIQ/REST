#include <crow.h>

#include <iostream>
#include <string>
#include <dotenv/dotenvFind.h>
#include "include/Database.h"

using namespace std;

int main()
{
    crow::SimpleApp app;

    dotenvFind::init();
    // Use std::getenv("DATABASE_HOST") to get the environment variable

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

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

    CROW_ROUTE(app, "/authentication/sayHello/<string>")
    .methods("GET"_method)
    ([](string name){
        //Return a JSON response with 200 status code
        crow::json::wvalue x;
        x["message"] = "Hello " + name;
        x["name"] = name;
        x["status"] = "success 200";
        return crow::response{200, x};
    });

    CROW_ROUTE(app, "/authentication/sayHello/<string>")
    .methods("POST"_method)
    ([](string name){
        //Return a JSON response with 201 status code
        crow::json::wvalue x;
        x["message"] = "Hello " + name;
        x["name"] = name;
        x["status"] = "success 201";
        return crow::response{201, x};
    });

    app.port(18080).multithreaded().run();
}