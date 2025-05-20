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

inline void register_workerRoutes(crow::App<JWTMiddleware> &app) {

    /**
     * @api {get} /worker/assignedTasks Get assigned tasks
     */
    CROW_ROUTE(app, "/worker/assignedTasks")
    .methods("GET"_method)
    ([&app](const crow::request &req) {
        auto &ctx = app.get_context<JWTMiddleware>(req);
        if (auto error = ctx.hasAnyRole({"worker"})) return std::move(*error);

        crow::json::wvalue res;
        res["success"] = false;

        string userId = ctx.getUserId();

        Database db;
        if (!db.connect()) {
            res["error"] = "Unexpected error"; return crow::response(500, res);
        }
        auto conn = db.getConn();

        auto stmnt = conn->prepareStatement("SELECT id, name, status, dueDate, priority FROM tasks WHERE assignedWorkerId = ? AND status IN ('ongoing', 'planned') ORDER BY dueDate ASC");
        stmnt->setInt(1, stoi(userId));
        auto result = stmnt->executeQuery();

        //When no tasks are found, just return an empty array
        vector<crow::json::wvalue> tasks;

        while (result->next()) {
            crow::json::wvalue task;
            task["id"] = result->getInt("id");
            task["name"] = result->getString("name");
            task["status"] = result->getString("status");
            task["dueDate"] = result->getString("dueDate");
            task["priority"] = result->getString("priority");

            tasks.push_back(task);
        }

        res["tasks"] = tasks;
        res["success"] = true;

        return crow::response(200, res);
    });

    /**
     * @api {get} /worker/assignedTasks/:id Get assigned task by id
     */
    CROW_ROUTE(app, "/worker/assignedTasks/<int>")
    .methods("GET"_method)
    ([&app](const crow::request &req, int taskId) {
        auto &ctx = app.get_context<JWTMiddleware>(req);
        if (auto error = ctx.hasAnyRole({"worker"})) return std::move(*error);

        crow::json::wvalue res;
        res["success"] = false;
        string userId = ctx.getUserId();

        Database db;
        if (!db.connect()) {
            res["error"] = "Unexpected error"; return crow::response(500, res);
        }

        auto conn = db.getConn();
        auto stmnt = conn->prepareStatement("SELECT t1.id, t1.name, t1.status, t1.dueDate, t1.priority, t1.description, t2.id AS farmId, t2.name AS farmName FROM tasks t1 INNER JOIN farms t2 ON (t1.farm=t2.id) WHERE t1.assignedWorkerId = ? AND t1.id = ? LIMIT 1");
        stmnt->setInt(1, stoi(userId));
        stmnt->setInt(2, taskId);
        auto result = stmnt->executeQuery();

        if (!result->next()) {
            res["error"] = "Task not found"; return crow::response(404, res);
        }

        crow::json::wvalue task;
        task["id"] = result->getInt("id");
        task["name"] = result->getString("name");
        task["status"] = result->getString("status");
        task["dueDate"] = result->getString("dueDate");
        task["priority"] = result->getString("priority");
        task["description"] = result->getString("description");
        task["farmId"] = result->getInt("farmId");
        task["farmName"] = result->getString("farmName");

        res["task"] = std::move(task);
        res["success"] = true;
        return crow::response(200, res);
    });

    /**
     * @api {put} /worker/assignedTasks/:id/status/:status Update task status
     */
    CROW_ROUTE(app, "/worker/assignedTasks/<int>/status/<str>")
    .methods("PUT"_method)
    ([&app](const crow::request &req, int taskId, string status) {
        auto &ctx = app.get_context<JWTMiddleware>(req);
        if (auto error = ctx.hasAnyRole({"worker"})) return std::move(*error);

        crow::json::wvalue res;
        res["success"] = false;
        string userId = ctx.getUserId();

        Database db;
        if (!db.connect()) {
            res["error"] = "Unexpected error"; return crow::response(500, res);
        }

        vector<string> validStatuses = {"planned", "ongoing", "ended"};
        if (find(validStatuses.begin(), validStatuses.end(), status) == validStatuses.end()) {
            res["error"] = "Invalid status"; return crow::response(400, res);
        }

        auto conn = db.getConn();
        auto stmnt = conn->prepareStatement("UPDATE tasks SET status = ? WHERE id = ? AND assignedWorkerId = ? LIMIT 1");
        stmnt->setString(1, status);
        stmnt->setInt(2, taskId);
        stmnt->setInt(3, stoi(userId));
        int updateCount = stmnt->executeUpdate();

        if (updateCount == 0) {
            res["error"] = "Task not found"; return crow::response(404, res);
        }

        res["success"] = true;
        return crow::response(200, res);
    });


    /**
     * @api {post} /farmer/tasks Create task
     */
    CROW_ROUTE(app, "/farmer/tasks")
    .methods("POST"_method)
    ([&app](const crow::request &req) {
        auto &ctx = app.get_context<JWTMiddleware>(req);
        if (auto error = ctx.hasAnyRole({"worker-farmer"})) return std::move(*error);

        crow::json::wvalue res;
        res["success"] = false;

        auto body = crow::json::load(req.body);
        if (!body) {
            res["error"] = "Invalid JSON"; return crow::response(400, res);
        }

        string userId = ctx.getUserId();

        Database db;
        if (!db.connect()) {
            res["error"] = "Unexpected error"; return crow::response(500, res);
        }

        // Check if all required fields are present
        if (!body["name"] || !body["assignedWorkerId"] || !body["dueDate"] || !body["priority"] || !body["description"] || !body["farmId"]) {
            res["error"] = "Missing required fields"; return crow::response(400, res);
        }

        auto conn = db.getConn();
        // Check if the assigned worker exists and is valid
        auto stmnt = conn->prepareStatement("SELECT id FROM linkedfarms WHERE userId = ? AND farmId = ? LIMIT 1");
        stmnt->setInt(1, body["assignedWorkerId"].i());
        stmnt->setInt(2, body["farmId"].i());
        auto result = stmnt->executeQuery();

        if (!result->next()) {
            res["error"] = "Invalid assigned worker"; return crow::response(400, res);
        }

        auto stmnt = conn->prepareStatement("INSERT INTO tasks (name, assignedWorkerId, status, dueDate, priority, description, farmId) VALUES (?, ?, ?, ?, ?, ?, ?)");
        stmnt->setString(1, string(body["name"].s()));
        stmnt->setInt(2, body["assignedWorkerId"].i());
        stmnt->setString(3, "planned");
        stmnt->setString(4, string(body["dueDate"].s()));
        stmnt->setString(5, string(body["priority"].s()));
        stmnt->setString(6, string(body["description"].s()));
        stmnt->setInt(7, body["farmId"].i());
        int insertCount = stmnt->executeUpdate();

        if (insertCount == 0) {
            res["error"] = "Task not created"; return crow::response(500, res);
        }

        res["success"] = true;
        return crow::response(200, res);
    });

    /**
     * @api {get} /farmer/farms Get farms
     */
    CROW_ROUTE(app, "/farmer/farms")
    .methods("GET"_method)
    ([&app](const crow::request &req) {
        auto &ctx = app.get_context<JWTMiddleware>(req);
        if (auto error = ctx.hasAnyRole({"worker-farmer"})) return std::move(*error);

        crow::json::wvalue res;
        res["success"] = false;

        Database db;
        if (!db.connect()) {
            res["error"] = "Unexpected error"; return crow::response(500, res);
        }

        string userId = ctx.getUserId();
        auto conn = db.getConn();

        auto stmnt = conn->prepareStatement("SELECT t1.id, t1.name, t1.usersCount FROM farms t1 INNER JOIN linkedfarms t2 ON (t1.id=t2.farmId AND t2.userId=?)");
        stmnt->setInt(1, stoi(userId));
        auto result = stmnt->executeQuery();

        vector<crow::json::wvalue> farms;
        while (result->next()) {
            crow::json::wvalue farm;
            farm["id"] = result->getInt("id");
            farm["name"] = result->getString("name");
            farm["usersCount"] = result->getInt("usersCount");

            farms.push_back(farm);
        }
        res["farms"] = farms;
        res["success"] = true;
        return crow::response(200, res);
    });

    /**
     * @api {get} /farmer/farms/:id Get farm by id
     */
    CROW_ROUTE(app, "/farmer/farms/<int>")
    .methods("GET"_method)
    ([&app](const crow::request &req, int farmId) {
        auto &ctx = app.get_context<JWTMiddleware>(req);
        if (auto error = ctx.hasAnyRole({"worker-farmer"})) return std::move(*error);

        crow::json::wvalue res;
        res["success"] = false;

        Database db;
        if (!db.connect()) {
            res["error"] = "Unexpected error"; return crow::response(500, res);
        }

        string userId = ctx.getUserId();
        auto conn = db.getConn();

        auto stmnt = conn->prepareStatement("SELECT t1.id, t1.name, t1.usersCount FROM farms t1 INNER JOIN linkedfarms t2 ON (t1.id=t2.farmId AND t2.userId=?) WHERE t1.id = ? LIMIT 1");
        stmnt->setInt(1, stoi(userId));
        stmnt->setInt(2, farmId);
        auto result = stmnt->executeQuery();

        if (!result->next()) {
            res["error"] = "Farm not found"; return crow::response(404, res);
        }

        crow::json::wvalue farm;
        farm["id"] = result->getInt("id");
        farm["name"] = result->getString("name");
        farm["usersCount"] = result->getInt("usersCount");

        res["farm"] = std::move(farm);
        res["success"] = true;
        return crow::response(200, res);
    });

    /**
     * @api {post} /farmer/farms Create farm
     */
    CROW_ROUTE(app, "/farmer/farms")
    .methods("POST"_method)
    ([&app](const crow::request &req) {
        auto &ctx = app.get_context<JWTMiddleware>(req);
        if (auto error = ctx.hasAnyRole({"worker-farmer"})) return std::move(*error);

        crow::json::wvalue res;
        res["success"] = false;

        auto body = crow::json::load(req.body);
        if (!body) {
            res["error"] = "Invalid JSON"; return crow::response(400, res);
        }

        string userId = ctx.getUserId();

        Database db;
        if (!db.connect()) {
            res["error"] = "Unexpected error"; return crow::response(500, res);
        }

        // Check if all required fields are present
        if (!body["name"]) {
            res["error"] = "Missing required fields"; return crow::response(400, res);
        }

        auto conn = db.getConn();
        auto stmnt = conn->prepareStatement("INSERT INTO farms (name) VALUES (?)");
        stmnt->setString(1, string(body["name"].s()));
        int insertCount = stmnt->executeUpdate();

        if (insertCount == 0) {
            res["error"] = "Farm not created"; return crow::response(500, res);
        }

        auto generatedKeys = stmnt->getGeneratedKeys();
        int farmId = 0;
        if (generatedKeys->next()) {
            farmId = generatedKeys->getInt(1);
        } else {
            res["error"] = "Failed to retrieve farm ID"; return crow::response(500, res);
        }

        // Link the farm to the user
        auto stmnt2 = conn->prepareStatement("INSERT INTO linkedfarms (userId, farmId) VALUES (?, ?)");
        stmnt2->setInt(1, stoi(userId));
        stmnt2->setInt(2, farmId);
        stmnt2->executeQuery();

        res["success"] = true;
        return crow::response(200, res);
    });
    
}