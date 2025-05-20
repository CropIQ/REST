#include "Database.h"
#include <iostream>

Database::Database() {
    urlEnv = std::getenv("DB_URL");
    if (urlEnv.empty()) {
        throw std::runtime_error("Environment variable DB_URL not set!");
    }
}

bool Database::connect() {
    try {
        sql::Driver* driver = sql::mariadb::get_driver_instance();
        sql::SQLString url(urlEnv);
        sql::Properties properties;
        connection = std::shared_ptr<sql::Connection>(driver->connect(url, properties));
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "Connection failed: " << e.what() << "\n";
        return false;
    }
}

std::shared_ptr<sql::Connection> Database::getConn() {
    return connection;
}

std::unique_ptr<sql::Statement> Database::createStatement() {
    try {
        return std::unique_ptr<sql::Statement>(connection->createStatement());
    } catch (sql::SQLException& e) {
        std::cerr << "Failed to create Statement: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<sql::PreparedStatement> Database::prepareStatement(const std::string& query) {
    try {
        return std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement(query));
    } catch (sql::SQLException& e) {
        std::cerr << "Failed to prepare Statement: " << e.what() << std::endl;
        return nullptr;
    }
}

Database::~Database() {
    if (connection && !connection->isClosed()) {
        connection->close();
    }
}