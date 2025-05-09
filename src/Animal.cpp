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
time_t Animal::getBirthDate() const { return birthDate; }

nlohmann::json Animal::getAnimalInfo() const {
   nlohmann::json infoJson;

   infoJson["id"] = animalId;
   infoJson["name"] = name;
   infoJson["weight"] = weight;
   
   char buffer[80];
   struct tm* timeinfo = std::localtime(&birthDate);
   strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
   infoJson["birth_date"] = buffer;

   return infoJson;
}

nlohmann::json Animal::getMedicamentHistory() const { // TODO: define method to get data from DB
}


void Animal::setGroupId(const int& groupId) { this->groupId = groupId; }
void Animal::setName(const std::string& name) { this->name = name; }
void Animal::setWeight(const int& weight) { this->weight = weight; }
void Animal::setBirthDate(const time_t& birthDate) { this->birthDate = birthDate; }



bool Animal::operator<(const Animal& other) const {
   this->name < other.name;
}
