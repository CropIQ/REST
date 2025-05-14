#pragma once

#include <vector>
#include <string>
#include "Item.h"
#include <nlohmann/json.hpp>

class Order {
private:
    int id;
    std::string orderDate;
    int itemId;
    int quantity;
    
public:
    Order();
    Order(const nlohmann::json& json);
    ~Order();

    nlohmann::json toJson() const;
};
