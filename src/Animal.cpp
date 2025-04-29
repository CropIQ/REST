#include "Medicament.h"
#include "Animal.h"
#include <string>
#include <ctime>
#include <vector>
#include <utility>
#include <sstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using std::string;

int Animal::nextId = 2000; // Animal ID first digit = 2

Animal::Animal(const string& name) : name(name) {
   animalId = nextId++;
}

Animal::~Animal() = default;

int Animal::getAnimalId() const { return animalId; }

string Animal::getName() const { return name; }

int Animal::getWeight() const { return weight; }

std::time_t Animal::getBirthDate() const { return birthDate; }

json Animal::getAnimalInfo() const {
   json info;

   info["id"] = animalId;
   info["name"] = name;
   info["weight"] = weight;
   
   char buffer[80];
   struct tm* timeinfo = std::localtime(&birthDate);
   strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
   info["birth_date"] = buffer;

   return info;
}

json Animal::getMedicamentHistory() const { // TODO: define method to get data from DB
   
}


void Animal::setName(const string& name) { this->name = name; }

void Animal::setWeight(const int& weight) { this->weight = weight; }

void Animal::setBirthDate(const time_t& birthDate) { this->birthDate = birthDate; }


void Animal::recordMedicament(const Medicament& med, int amountOfMedicament) { // TODO: define method to send data to DB
}

bool Animal::operator<(const Animal& other) const {
   this->name < other.name;
}
