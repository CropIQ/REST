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

static string generateAccessToken(const string &userid, shared_ptr<sql::Connection> &conn) {
    // Search for user in DB and get user role

    auto stmnt = conn->prepareStatement("SELECT role, name, email FROM users WHERE id = ? LIMIT 1");
    stmnt->setString(1, userid);
    auto result = stmnt->executeQuery();
    if (!result->next()) {
        return "";
    }


    string userrole = result->getString("role").c_str();
    string email = result->getString("email").c_str();
    string name = result->getString("name").c_str();

    auto now = chrono::system_clock::now();
    auto accessToken = jwt::create()
        .set_type("JWS")
        .set_issuer("CropIQ")
        .set_payload_claim("userid", jwt::claim(userid))
        .set_payload_claim("email", jwt::claim(email))
        .set_payload_claim("role", jwt::claim(userrole))
        .set_payload_claim("name", jwt::claim(name))
        .set_expires_at(now + chrono::minutes(15))
        .sign(jwt::algorithm::hs256{std::getenv("JWT_SECRET_KEY")});

    // Return the access token
    return accessToken;
}

inline void register_authRoutes(crow::App<JWTMiddleware> &app) {

    CROW_ROUTE(app, "/login")
    .methods("POST"_method)
    ([](const crow::request &req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            crow::json::wvalue res; res["error"] = "Invalid JSON";
            return crow::response(400, res);
        }

        string email = body["email"].s();
        string password = body["password"].s();
        string hashedPassword = hmac_sha256(std::getenv("PASSWORD_HASH_KEY"), password);

        //DB check
        Database db;
        if (!db.connect()) return crow::response(500, "Unexpected error");
        auto conn = db.getConn();

        //Use prepared statements to prevent SQL injection
        auto stmnt = conn->prepareStatement("SELECT id FROM users WHERE email = ? AND password = ? LIMIT 1");
        stmnt->setString(1, email);
        stmnt->setString(2, hashedPassword);
        auto result = stmnt->executeQuery();
        if (result->next()) {
            auto now = chrono::system_clock::now();

            string userid = result->getString("id").c_str();
            string accessToken = generateAccessToken(userid, conn);

            if (accessToken.empty()) {
                // Return error response as json
                crow::json::wvalue res; res["error"] = "Failed to generate access token";
                return crow::response{500, res};
            }

            auto refreshToken = jwt::create()
                .set_type("JWS")
                .set_issuer("CropIQ-RT")
                .set_payload_claim("userid", jwt::claim(userid))
                .set_expires_at(now + chrono::hours(14*24))
                .sign(jwt::algorithm::hs256{std::getenv("JWT_SECRET_KEY")});

            crow::json::wvalue res;
            res["accessToken"] = accessToken;
            res["refreshToken"] = refreshToken;
            return crow::response{res};
        }

        crow::json::wvalue res; res["error"] = "Unauthorized";
        return crow::response(401, res);
    });

    CROW_ROUTE(app, "/register")
    .methods("POST"_method)
    ([](const crow::request &req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            crow::json::wvalue res; res["error"] = "Invalid JSON";
            return crow::response(400, res);
        }

        vector<string> availableRoles = {"worker"};

        string name = body["name"].s();
        string role = body["role"].s();
        string email = body["email"].s();
        string password = body["password"].s();
        string hashedPassword;
        Database db;

        int errorsCount = 0;
        vector<string> errors;

        if (name.empty()) { errors.push_back("Name is required"); errorsCount++; }
        if (role.empty()) { errors.push_back("Role is required"); errorsCount++; }
        if (email.empty()) { errors.push_back("Email is required"); errorsCount++; }
        if (password.empty()) { errors.push_back("Password is required"); errorsCount++; }

        if (errorsCount == 0) {
            if (find(availableRoles.begin(), availableRoles.end(), role) == availableRoles.end()) {
                errors.push_back("Role is not valid"); errorsCount++;
            }

            if (email.find('@') == string::npos) {
                errors.push_back("Email is not valid"); errorsCount++;
            }

            if (password.length() < 8) {
                errors.push_back("Password must be at least 8 characters long"); errorsCount++;
            }

            if (password.find_first_of("0123456789") == string::npos) {
                errors.push_back("Password must contain at least one digit"); errorsCount++;
            }

            if (password.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos) {
                errors.push_back("Password must contain at least one uppercase letter"); errorsCount++;
            }

            if (password.find_first_of("abcdefghijklmnopqrstuvwxyz") == string::npos) {
                errors.push_back("Password must contain at least one lowercase letter"); errorsCount++;
            }

            if (password.find_first_of("!@#$%^&*()_+-=[]{}|;':\",.<>?/") == string::npos) {
                errors.push_back("Password must contain at least one special character"); errorsCount++;
            }
        }

        if (errorsCount == 0) {
            hashedPassword = hmac_sha256(std::getenv("PASSWORD_HASH_KEY"), password);
            //DB check
            if (!db.connect()) return crow::response(500, "Unexpected error");
            auto conn = db.getConn();
            auto stmnt = conn->prepareStatement("SELECT id FROM users WHERE email = ? LIMIT 1");
            stmnt->setString(1, email);
            auto result = stmnt->executeQuery();
            if (result->next()) {
                errors.push_back("Email already exists"); errorsCount++;
            }
        }

        string userid;

        if (errorsCount == 0) {
            //DB insert
            auto conn = db.getConn();
            auto stmnt = conn->prepareStatement("INSERT INTO users (name, email, password, role) VALUES (?, ?, ?, ?)");
            stmnt->setString(1, name);
            stmnt->setString(2, email);
            stmnt->setString(3, hashedPassword);
            stmnt->setString(4, role);
            stmnt->executeQuery();
            
            // SELECT LAST_INSERT_ID()
            auto stmt = conn->createStatement();
            auto result = stmt->executeQuery("SELECT LAST_INSERT_ID()");

            if (!result->next()) {
                errors.push_back("Failed to get user ID"); errorsCount++;
            } else {
                userid = result->getString(1).c_str();
            }       
        }

        if (errorsCount == 0) {
            auto conn = db.getConn();
            string accessToken = generateAccessToken(userid, conn);
            string refreshToken = jwt::create()
                .set_type("JWS")
                .set_issuer("CropIQ-RT")
                .set_payload_claim("userid", jwt::claim(userid))
                .set_expires_at(chrono::system_clock::now() + chrono::hours(14*24))
                .sign(jwt::algorithm::hs256{std::getenv("JWT_SECRET_KEY")});

            if (accessToken.empty()) {
                crow::json::wvalue res; res["error"] = "Failed to generate access token";
                return crow::response{500, res};
            }

            crow::json::wvalue res;
            res["accessToken"] = accessToken;
            res["refreshToken"] = refreshToken;
            return crow::response{res};
        } else {
            crow::json::wvalue res; res["error"] = errors;
            return crow::response(400, res);
        }

    });
        

    CROW_ROUTE(app, "/refreshToken")
    .methods("POST"_method)
    ([](const crow::request &req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("refreshToken")) {
            crow::json::wvalue res; res["error"] = "Invalid JSON or missing refresh token";
            return crow::response(400, res);
        }

        string refreshToken = body["refreshToken"].s();

        try {
            auto decoded = jwt::decode(refreshToken);
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{std::getenv("JWT_SECRET_KEY")})
                .with_issuer("CropIQ");

            verifier.verify(decoded);

            string userid = decoded.get_payload_claim("userid").as_string();

            // Generate a new access token
            Database db;
            if (!db.connect()) return crow::response(500, "Unexpected error");
            auto conn = db.getConn();
            string accessToken = generateAccessToken(userid, conn);
            if (accessToken.empty()) {
                crow::json::wvalue res; res["error"] = "Failed to generate access token";
                return crow::response(500, res);
            }

            crow::json::wvalue res;
            res["accessToken"] = accessToken;
            return crow::response{res};

        } catch (...) {
            crow::json::wvalue res; res["error"] = "Invalid refresh token";
            return crow::response(401, res);
        }
    });
}