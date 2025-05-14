#include "Inventory.h"

Inventory::Inventory(const std::string& name) :
    name(name) {}

Inventory::Inventory(const nlohmann::json& json) {
    name = json.at("name").get<std::string>();
}

Inventory::~Inventory() = default;

void Inventory::addItem(const Item& item, int quantity) {
    for (auto& invItem : items) {
        if (invItem.getId() == item.getId()) {
            invItem.changeAmount(invItem.getQuantity() + quantity);
            return;
        }
    }
    Item newItem(item.getId(), item.getName(), quantity);
    items.push_back(newItem);
}

void Inventory::removeItem(const Item& item, int quantity) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->getId() == item.getId()) {
            if (it->getQuantity() > quantity) {
                it->changeAmount(it->getQuantity() - quantity);
            } else {
                items.erase(it);
            }
            return;
        }
    }
}

std::vector<Item> Inventory::viewInventory() const { return items; }
int Inventory::getId() const { return id; }
std::string Inventory::getName() const { return name; }

nlohmann::json Item::toJson() const {
    nlohmann::json jsonData;
    jsonData["id"] = id;
    jsonData["name"] = name;

    // If Inventory contains a vector of Items
    jsonData["items"] = nlohmann::json::array();
    for (const auto& item : items) {
        jsonData["items"].push_back(item.toJson()); // Assuming Item has a toJson() method
    }

    return jsonData;
}
