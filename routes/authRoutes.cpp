#pragma once

#include <crow.h>

#include <iostream>
#include <string>

#include "jwt-cpp/jwt.h"

using namespace std;

static string generateAccessToken(const string &userid) {
    // Search for user in DB and get user role

    string userrole = "admin"; // Replace with actual user role
    string email = "mail@mail.com"; // Replace with actual email

    auto now = chrono::system_clock::now();
    auto accessToken = jwt::create()
        .set_type("JWS")
        .set_issuer("CropIQ")
        .set_payload_claim("userid", jwt::claim(userid))
        .set_payload_claim("email", jwt::claim(email))
        .set_payload_claim("role", jwt::claim(userrole))
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

        // Dummy check
        if (email == "admin" && password == "password") {
            auto now = chrono::system_clock::now();

            string userid = "ID"; // Replace with actual user ID

            string accessToken = generateAccessToken(userid);

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
            string accessToken = generateAccessToken(userid);
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