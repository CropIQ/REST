#include "Animal.h"
#include "Medicament.h"
#include <string>
#include <ctime>
#include <vector>
#include <utility>
#include <sstream>


using std::string;

Animal::Animal(const string& name) : name(name) {}

Animal::~Animal() {} // TODO: create proper destructor

int Animal::getAnimalID() const { return animalID; }

string Animal::getName() const { return name; }

int Animal::getWeight() const { return weight; }

string Animal::getMedicamentHistory() const { 
   std::stringstream ss;
   for(auto it = medicaments.begin(); it < medicaments.end(); ++it) {
      ss << (*it).first.getName() << (*it).second << std::endl;
   }
   return ss.str();
}

void Animal::recordMedicament(const Medicament& med, int amountOfMedicament) {
   medicaments.push_back(std::make_pair(med, amountOfMedicament));
}