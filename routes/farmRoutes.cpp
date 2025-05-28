#pragma once

#include <crow.h>

#include <iostream>
#include <string>

#include <crow/middlewares/cors.h>
#include "jwt-cpp/jwt.h"
#include <dotenv/dotenvFind.h>

#include "../include/Encryption.h"
#include "../include/Database.h"
#include "../include/middleware/JWTMiddleware.h"

using namespace std;

void register_farmRoutes(crow::App<crow::CORSHandler, JWTMiddleware> &app) {

   // Get all farms
   CROW_ROUTE(app, "/farms")
   .methods("GET"_method)
   ([&app](const crow::request &req) {
      auto &ctx = app.get_context<JWTMiddleware>(req);
      if (auto error = ctx.hasAnyRole({"worker"})) return std::move(*error);

      crow::json::wvalue res;
      res["success"] = false;

      Database db;
      if (!db.connect()) {
         res["error"] = "Unexpected error";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      auto stmnt = conn->prepareStatement("SELECT t2.id, t2.name FROM linkedfarms as t1 INNER JOIN farms as t2 ON (t1.farmId=t2.id) WHERE t1.userId = ?");
      stmnt->setInt(1, stoi(ctx.getUserId()));
      auto result = stmnt->executeQuery();

      std::vector<crow::json::wvalue> farmList;

      while (result->next()) {
         crow::json::wvalue farm;
         farm["id"] = result->getInt("id");
         farm["name"] = result->getString("name");
         farmList.push_back(std::move(farm));
      }
      res["success"] = true;
      res["farms"] = std::move(farmList);
      return crow::response(200, res);
   });


   // Create new farm
   CROW_ROUTE(app, "/farms")
   .methods("POST"_method)
   ([&app](const crow::request &req) {
      auto &ctx = app.get_context<JWTMiddleware>(req);
      if (auto error = ctx.hasAnyRole({"worker"})) return std::move(*error);

      crow::json::wvalue res;
      res["success"] = false;
      
      auto body = crow::json::load(req.body);
      if (!body) {
         res["error"] = "Invalid JSON";
         return crow::response(400, res);
      }
      
      string name = body["name"].s();
      if (name.empty() || name.length() > 100) {
         res["error"] = "Invalid farm name";
         return crow::response(400, res);
      }
      
      // Database check
      Database db;
      if (!db.connect()) {
         res["error"] = "Unexpected error";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      auto stmnt = conn->prepareStatement("INSERT INTO farms (name, usersCount) VALUES (?, 1)", sql::Statement::RETURN_GENERATED_KEYS);
      stmnt->setString(1, name);
      stmnt->executeUpdate();

      auto generatedKeys = stmnt->getGeneratedKeys();
      if (!generatedKeys->next()) {
         res["error"] = "Farm not created";
         return crow::response(500, res);
      }

      int newFarmId = generatedKeys->getInt(1);
      
      // Link farm to user
      auto linkStmt = conn->prepareStatement("INSERT INTO linkedfarms (userId, farmId) VALUES (?, ?)");
      linkStmt->setInt(1, stoi(ctx.getUserId()));
      linkStmt->setInt(2, newFarmId);
      linkStmt->executeUpdate();

      res["success"] = true;
      res["id"] = newFarmId;
      return crow::response(201, res);
   });

   // Get farm from database by ID
   CROW_ROUTE(app, "/farms/<int>")
   .methods("GET"_method)
   ([&app](const crow::request &req, int id) {
      auto &ctx = app.get_context<JWTMiddleware>(req);
      if (auto error = ctx.hasAnyRole({"worker"})) return std::move(*error);

      crow::json::wvalue res;
      res["success"] = false;

      Database db;
      if (!db.connect()) {
         res["error"] = "Unexpected error";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      auto stmnt = conn->prepareStatement("SELECT t1.id, t1.name, t1.usersCount FROM farms as t1 INNER JOIN linkedfarms as t2 ON (t1.id=t2.farmId AND t2.userId=?) WHERE t1.id = ? LIMIT 1");
      stmnt->setInt(1, stoi(ctx.getUserId()));
      stmnt->setInt(2, id);
      auto result = stmnt->executeQuery();

      crow::json::wvalue farm;
      if (!result->next()) {
         farm["error"] = "Farm not found";
         return crow::response(404, farm);
      }

      farm["id"] = result->getInt("id");
      farm["name"] = result->getString("name");
      farm["usersCount"] = result->getInt("usersCount");

      res["success"] = true;
      res["farm"] = std::move(farm);

      return crow::response(res);
   });

   // Update farm name
   CROW_ROUTE(app, "/farms/<int>")
   .methods("PUT"_method)
   ([&app](const crow::request& req, int id) {
      auto &ctx = app.get_context<JWTMiddleware>(req);
      if (auto error = ctx.hasAnyRole({"worker"})) return std::move(*error);

      crow::json::wvalue res;
      res["success"] = false;

      auto body = crow::json::load(req.body);
      if(!body) {
         res["error"] = "Invalid JSON";
         return crow::response(400, res);
      }

      std::string newName = body["name"].s();
      if (newName.empty() || newName.length() > 100) {
         res["error"] = "Invalid farm name";
         return crow::response(400, res);
      }

      Database db;
      if (!db.connect()) {
         res["error"] = "Unexpected error";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      try {
         auto stmnt = conn->prepareStatement("UPDATE farms as t1 INNER JOIN linkedfarms as t2 ON (t1.id=t2.farmId AND t2.userId=?) SET t1.name = ? WHERE t1.id = ?");
         stmnt->setInt(1, stoi(ctx.getUserId()));
         stmnt->setString(2, newName);
         stmnt->setInt(3, id);
         int affectedRows = stmnt->executeUpdate();

         if (affectedRows == 0) {
            res["error"] = "Farm not found or name not changed";
            return crow::response(404, res);
         }

         res["success"] = true;
         return crow::response(200, res);

      } catch(const sql::SQLException& e) {
         std::cerr << "SQL error: " << e.what() << std::endl;

         res["error"] = "Unexpected error";
         return crow::response(500, res);
      }
   });

   CROW_ROUTE(app, "/farms/<int>")
   .methods("DELETE"_method)
   ([&app](const crow::request& req, int id) {
      auto &ctx = app.get_context<JWTMiddleware>(req);
      if (auto error = ctx.hasAnyRole({"worker"})) return std::move(*error);

      crow::json::wvalue res;
      res["success"] = false;

      Database db;
      if (!db.connect()) {
         res["error"] = "Unexpected error";
         return crow::response(500, res);
      }

      auto conn = db.getConn();
      //auto stmnt = conn->prepareStatement("DELETE FROM farms WHERE id = ?");
      auto stmnt = conn->prepareStatement("DELETE t1 FROM farms as t1 INNER JOIN linkedfarms as t2 ON (t1.id=t2.farmId AND t2.userId=?) WHERE t1.id = ?");
      stmnt->setInt(1, stoi(ctx.getUserId()));
      stmnt->setInt(2, id);
      int affectedRows = stmnt->executeUpdate();

      if (affectedRows > 0) {
         res["success"] = true;
         return crow::response(200, res);
      } else {
         res["error"] = "Farm not found or already deleted";
         return crow::response(404, res);
      }
   });

}