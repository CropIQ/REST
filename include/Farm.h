#pragma once

#include <string>
#include <vector>
#include "Inventory.h"
#include "AnimalGroup.h"

class Farm {
private:
    int id;
    std::string name;
    std::vector<Inventory> inventories;
    std::vector<AnimalGroup> animalGroups;

public:
    Farm(int& id, const std::string& name);
    Farm(const nlohmann::json& json);
    ~Farm();

    void addAnimalGroup(const AnimalGroup& group);
    void addInventory(const Inventory& inventory);
    nlohmann::json toJson() const;

    int getId() const;
    std::string getName() const;
    std::vector<Inventory> getInventories() const;
    std::vector<AnimalGroup> getAnimalGroups() const;

};