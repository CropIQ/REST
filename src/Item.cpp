#include "Item.h"

Item::Item(const int& id, const std::string& name, const int& quantity) :
    itemID(id), 
    name(name), 
    quantity(quantity) {}

Item::Item(const nlohmann::json& json) {
    itemID = json.at("itemID").get<int>();
    name = json.at("name").get<std::string>();
    quantity = json.at("quantity").get<int>();
}

Item::~Item() = default;

void Item::changeAmount(int newQuantity) {
    quantity = newQuantity;
}

void Item::changeName(const std::string& newName) {
    name = newName;
}

void Item::orderItem() { //TODO when order class is created
    
}

int Item::getID() const { return itemID; }
std::string Item::getName() const { return name; }
int Item::getQuantity() const { return quantity; }

nlohmann::json Item::toJson() const {
    return {
        {"itemID", itemID},
        {"name", name},
        {"quantity", quantity}
    };
}


