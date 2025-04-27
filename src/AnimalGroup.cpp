#include "AnimalGroup.h"
#include "Animal.h"
#include <string>
#include <unordered_map>

using std::string;

int AnimalGroup::nextID = 3000; // AnimalGroup ID first digit = 3

AnimalGroup::AnimalGroup (const string& groupName) : 
   groupName(groupName),
   animalAmount(0) {}

int AnimalGroup::getGroupID() const { return groupID; }

int AnimalGroup::getAnimalAmount() const { return animalAmount; }

string AnimalGroup::getGroupName() const { return groupName; }

