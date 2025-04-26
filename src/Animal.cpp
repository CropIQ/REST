#include "Animal.h"
#include "Medicament.h"
#include <string>
#include <ctime>
#include <vector>
#include <utility>
#include <sstream>

using std::string;

int Animal::ID = 2000; // Animal ID first digit = 2

Animal::Animal(const string& name) : name(name) {
   animalID = ID++;
}

Animal::~Animal() {} // TODO: create proper destructor

int Animal::getAnimalID() const { return animalID; }

string Animal::getName() const { return name; }

int Animal::getWeight() const { return weight; }

std::time_t Animal::getBirthDate() const { return birthDate; }


void Animal::setName(const string& newName) { name = newName; }

void Animal::setWeight(const int& newWeight) { weight = newWeight; }

void Animal::setBirthDate(const time_t& newBirthDate) { birthDate = newBirthDate; }

void Animal::recordMedicament(const Medicament& med, int amountOfMedicament) { 
   medicaments.push_back(std::make_pair(med, amountOfMedicament));
}


string Animal::getMedicamentHistory() const { 
   std::stringstream ss;   
   for(auto it = medicaments.begin(); it < medicaments.end(); ++it) {
      ss << (*it).first.getName() << (*it).second << std::endl;
   }
   return ss.str();
}
