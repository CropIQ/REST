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
   
   // Create new farm
   CROW_ROUTE(app, "/farm")
   .methods("POST"_method)
   ([](const crow::request &req) {
      Database db;
      
      auto body = crow::json::load(req.body);
      if (!body) {
         crow::json::wvalue res; res["error"] = "Invalid JSON";
         return crow::response(400, res);
      }

      string name = body["name"].s();
      int usersCount = body["usersCount"].i();

      Database db;
      if (!db.connect()) {
         crow::json::wvalue res; res["error"] = "Database connection failed";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      auto stmnt = conn->prepareStatement("INSERT INTO farms (name, usersCount) VALUES (?, ?)");
      stmnt->setString(1, name);
      stmnt->setInt(2, usersCount);
      stmnt->executeUpdate();

      auto stmt = conn->createStatement();
      auto result = stmt->executeQuery("SELECT LAST_INSERT_ID()");
      string farmId = (result->next()) ? result->getString(1).c_str() : "unknown";

      crow::json::wvalue res;
      res["message"] = "Farm created";
      res["id"] = farmId;
      return crow::response(201, res);
   });

   // Get farm from database by ID
   CROW_ROUTE(app, "/farm")
   .methods("GET"_method)
   ([](const crow::request &req) {
      Database db;
      if (!db.connect()) {
         crow::json::wvalue res; res["error"] = "Database connection failed";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      auto stmnt = conn->createStatement();
      auto result = stmnt->executeQuery("SELECT id, name, usersCount FROM farms WHERE id = ?");

      crow::json::wvalue farms;
      for(int i = 0; result->next(); ++i) {
         farms[i]["id"] = result->getInt("id");
         farms[i]["name"] = result->getString("name");
         farms[i]["usersCount"] = result->getInt("usersCount");
      }

      return crow::response(farms);
   });

   // Update farm name
   CROW_ROUTE(app, "/farm/<int>")
   .methods("PUT"_method)
   ([&](const crow::request& req, int id) {
      auto body = crow::json::load(req.body);
      if(!body) {
         crow::json::wvalue res; res["error"] = "Invalid JSON";
         return crow::response(400, res);
      }

      std::string newName = body["name"].s();

      Database db;
      if (!db.connect()) {
         crow::json::wvalue res; res["error"] = "Database connection failed";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      try {
         auto stmnt = conn->prepareStatement("UPDATE farms SET name = ? WHERE id = ?");
         stmnt->setString(1, newName);
         stmnt->setInt(2, id);

         int affectedRows = stmnt->executeUpdate();
         if(!affectedRows) {
            crow::json::wvalue res; res["error"] = "Farm not found or name not changed";
            return crow::response(404, res);
         }

         crow::json::wvalue res; res["message"] = "Farm name updated succesfully";
         return crow::response(200, res);

      } catch(const sql::SQLException& e) {
         crow::json::wvalue res;
         res["error"] = "SQL error: " + std::string(e.what());
         return crow::response(500, res);
      }
   });

   CROW_ROUTE(app, "/farm/<int>")
   .methods("DELETE"_method)
   ([&](int id) {
      Database db;
      if (!db.connect()) {
         crow::json::wvalue res; res["error"] = "Database connection failed";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      auto stmnt = conn->prepareStatement("DELETE FROM farms WHERE id = ?");
      stmnt->setInt(1, id);
      int affectedRows = stmnt->executeUpdate();

      crow::json::wvalue res;
      if (affectedRows > 0) {
         res["message"] = "Farm deleted";
         return crow::response(200, res);
      } else {
         res["error"] = "Farm not found";
         return crow::response(404, res);
      }
   });

}