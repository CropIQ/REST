#include "Medicament.h"
#include "Animal.h"
#include <string>
#include <ctime>
#include <utility>
#include "nlohmann/json.hpp"

Animal::Animal(const int& Id, const int& groupId, const std::string& name, const int& weight) : 
   animalId(Id),
   groupId(groupId),
   name(name),
   weight(weight) {}

Animal::Animal(const nlohmann::json& json) {
   animalId = json.at("animalId").get<int>();
   groupId = json.at("groupId").get<int>();
   name = json.at("animalName").get<std::string>();
   weight = json.at("weight").get<int>();
}

Animal::~Animal() = default;

void Animal::recordMedicament(const Medicament& med, int amountOfMedicament) { // TODO: define method to send data to DB
}

int Animal::getAnimalId() const { return animalId; }
int Animal::getGroupId() const { return groupId; }
std::string Animal::getName() const { return name; }
int Animal::getWeight() const { return weight; }
std::string Animal::getBirthDateTime() const { return birthDateTime; }

nlohmann::json Animal::toJson() const {
   nlohmann::json infoJson;

   infoJson["id"] = animalId;
   infoJson["name"] = name;
   infoJson["weight"] = weight;
   infoJson["birth_date"] = birthDateTime;

   return infoJson;
}

nlohmann::json Animal::getMedicamentHistory() const { // TODO: define method to get data from DB
   nlohmann::json medicamentHistoryJson;
   for (const auto& medicament : medicaments) {
       medicamentHistoryJson.push_back({
           {"medicamentId", medicament.first.getId()},
           {"name", medicament.first.getName()},
           {"amount", medicament.second}
       });
   }
   return medicamentHistoryJson;
}


void Animal::setGroupId(const int& groupId) { this->groupId = groupId; }
void Animal::setName(const std::string& name) { this->name = name; }
void Animal::setWeight(const int& weight) { this->weight = weight; }
void Animal::setBirthDate(const std::string& birthDate) { this->birthDateTime = birthDate; }



bool Animal::operator<(const Animal& other) const {
   return name < other.name;
}
