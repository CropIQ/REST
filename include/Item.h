#pragma once

#include <string>
#include <nlohmann/json.hpp>

class Item {
private:
    int itemID;
    std::string name;
    int quantity;

public:
    Item(const int& id, const std::string& name, const int& quantity);
    Item(const nlohmann::json& json);
    ~Item();

    void changeAmount(int newQuantity);
    void changeName(const std::string& newName);
    void orderItem();
    nlohmann::json toJson() const;

    int getID() const;
    std::string getName() const;
    int getQuantity() const;

};