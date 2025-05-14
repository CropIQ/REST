#include "Item.h"

Item::Item(const int& id, const std::string& name, const int& quantity) :
    id(id), 
    name(name), 
    quantity(quantity) {}

Item::Item(const nlohmann::json& json) {
    id = json.at("id").get<int>();
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

int Item::getId() const { return id; }
std::string Item::getName() const { return name; }
int Item::getQuantity() const { return quantity; }

nlohmann::json Item::toJson() const {
    return {
        {"id", id},
        {"name", name},
        {"quantity", quantity}
    };
}


