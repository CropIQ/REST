#include "../../include/middleware/JWTMiddleware.h"

unique_ptr<crow::response> JWTMiddleware::context::hasAnyRole(const vector<string> &requiredRoles) {
    for (const auto &role : requiredRoles) {
        if (role == "" || role == userRole_) {
            return nullptr;
        }
    }

    crow::json::wvalue x;
    x["error"] = "Unauthorized";

    auto res = make_unique<crow::response>(401, x);
    return res;
}

string JWTMiddleware::context::getUserRole() const {
    return userRole_;
}

map<string, string> JWTMiddleware::context::getUserData() const {
    if (isAuthenticated) return userData_;
    return map<string, string>();
}



void JWTMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx){}
void JWTMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx) {
    // Validate the access token
    const auto& auth_header = req.get_header_value("Authorization");
    if (auth_header.empty() || auth_header.substr(0, 7) != "Bearer ") {
        return;
    }

    string token = auth_header.substr(7); // Extract token

    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{std::getenv("JWT_SECRET_KEY")})
            .with_issuer("CropIQ");

        verifier.verify(decoded);

        ctx.isAuthenticated = true;
        ctx.userRole_ = decoded.get_payload_claim("role").as_string();

        // Extract user data from the token and store in map<string, string> userData_;
        auto claims = decoded.get_payload_claims();
        for (const auto& kv : claims) {
            ctx.userData_[kv.first] = kv.second.as_string();
        }

    } catch (...) {
        return;
    }
}
