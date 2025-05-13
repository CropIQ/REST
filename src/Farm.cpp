#include "Farm.h"

Farm::Farm(int& id, const std::string& name) :
    id(id),
    name(name) {}

Farm::Farm(const nlohmann::json& json) {
    id = json.at("id").get<int>();
    name = json.at("name").get<std::string>();
    //possibly need to deserialize inventories n animal groups
}

Farm::~Farm() = default;


void Farm::addAnimalGroup(const AnimalGroup& group) {
    animalGroups.push_back(group);
}

void Farm::addInventory(const Inventory& inventory) {
    inventories.push_back(inventory);
}

int Farm::getId() const {
    return id;
}

std::string Farm::getName() const {
    return name;
}

std::vector<Inventory> Farm::getInventories() const {
    return inventories;
}

std::vector<AnimalGroup> Farm::getAnimalGroups() const {
    return animalGroups;
}

nlohmann::json Farm::toJson() const {
    nlohmann::json jsonData;
    jsonData["id"] = id;
    jsonData["name"] = name;

    //serialize inventories
    jsonData["inventories"] = nlohmann::json::array();
    for (const auto& inventory : inventories) {
        jsonData["inventories"].push_back(inventory.toJson()); // Assuming Inventory has a toJson() method
    }

    //serialize animal groups
    jsonData["animalGroups"] = nlohmann::json::array();
    for (const auto& group : animalGroups) {
        jsonData["animalGroups"].push_back(group.toJson()); // Assuming AnimalGroup has a toJson() method
    }

    return jsonData;
}