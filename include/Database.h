#pragma once
#include <mariadb/conncpp.hpp>
#include <dotenv/dotenvFind.h>
#include <memory>
#include <string>

class Database {
public:
    Database();
    bool connect();
    std::shared_ptr<sql::Connection> getConn();
    ~Database();
    std::unique_ptr<sql::Statement> createStatement();
    std::unique_ptr<sql::PreparedStatement> prepareStatement(const std::string& query);
    // Disable copy constructor and assignment operator
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
private:
    std::string urlEnv;
    std::shared_ptr<sql::Connection> connection;
};
