#include "AnimalGroup.h"
#include "Animal.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

using std::string;

int AnimalGroup::nextID = 3000; // AnimalGroup ID first digit = 3

AnimalGroup::AnimalGroup (const string& groupName) : 
   groupName(groupName),
   animalAmount(0) {}

int AnimalGroup::getGroupID() const { return groupID; }

int AnimalGroup::getAnimalAmount() const { return animalAmount; }

string AnimalGroup::getGroupName() const { return groupName; }

void AnimalGroup::addAnimal(Animal* newAnimal) {
   animalMap.insert({newAnimal->getAnimalID(), newAnimal});
   ++animalAmount;
}

void AnimalGroup::removeAnimal(Animal* removedAnimal) {
   animalMap.erase(removedAnimal->getAnimalID());
   --animalAmount;
}

void AnimalGroup::mergeGroup(AnimalGroup& otherGroup) { 
   for(const auto& [keyID, animal] : this->animalMap) {
      auto result = otherGroup.animalMap.insert({keyID, animal});
      if(result.second) ++(otherGroup.animalAmount);
   }
   this->animalMap.clear();
   this->animalAmount = 0;
}

string AnimalGroup::generateReport() const {
   std::vector<Animal*> animalList;
   
   for(const auto& [keyID, animalPtr] : animalMap) {
      animalList.push_back(animalPtr);
   }
   
   std::sort(animalList.begin(), animalList.end(),
      [](Animal* a, Animal* b) { *a < *b; });
   
   std::stringstream ss;
   for(const Animal *animal : animalList) {
      ss << animal->getAnimalInfo() << std::endl;
   }
   return ss.str();
}

