#include "AnimalGroup.h"
#include "Animal.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using std::string;

int AnimalGroup::nextId = 3000; // AnimalGroup ID first digit = 3

AnimalGroup::AnimalGroup(const string& groupName) : 
   groupId(nextId++),
   groupName(groupName),
   animalAmount(0) {}

AnimalGroup::~AnimalGroup() { animalMap.clear(); }

int AnimalGroup::getGroupId() const { return groupId; }

int AnimalGroup::getAnimalAmount() const { return animalAmount; }

string AnimalGroup::getGroupName() const { return groupName; }

void AnimalGroup::addAnimal(Animal* animal) {
   this->animalMap.insert({animal->getAnimalId(), animal});
   ++animalAmount;
}

void AnimalGroup::addAnimals(std::vector<Animal*> animals) {
   for(Animal* animal : animals) {
      animal->setGroupId(this->groupId);
      this->animalMap.insert({animal->getAnimalId(), animal});
      ++this->animalAmount;
   }
}

void AnimalGroup::removeAnimal(Animal* animal) {
   animal->setGroupId(0);
   this->animalMap.erase(animal->getAnimalId());
   --animalAmount;
}

void AnimalGroup::mergeGroup(AnimalGroup& otherGroup) { 
   for(const auto& [keyId, animal] : otherGroup.animalMap) {
      auto result = this->animalMap.insert({keyId, animal});
      if(result.second) {
         animal->setGroupId(this->groupId);
         ++(this->animalAmount);
         
      }
   }
   delete &otherGroup;
}

json AnimalGroup::generateReport() const {
   std::vector<Animal*> animalList;
   
   for(const auto& [keyId, animalPtr] : animalMap) {
      animalList.push_back(animalPtr);
   }
   
   std::sort(animalList.begin(), animalList.end(),
      [](Animal* a, Animal* b) { *a < *b; });
   
   json report = json::array();
   for(const Animal *animal : animalList) {
      report.push_back(animal->getAnimalInfo());
   }
   
   return report;
}

