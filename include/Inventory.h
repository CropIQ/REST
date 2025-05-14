#pragma once

#include <string>
#include <vector>
#include "Item.h"
#include <nlohmann/json.hpp>

class Inventory {
private:
    int id;
    std::vector<Item> items;
    std::string name;

public:
    Inventory(const std::string& name);
    Inventory(const nlohmann::json& json);
    ~Inventory();

    void addItem(const Item& item, int quantity);
    void removeItem(const Item& item, int quantity);
    nlohmann::json toJson() const;

    int getId() const;
    std::string getName() const;

};
