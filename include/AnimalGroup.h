#ifndef ANIMALGROUP_H
#define ANIMALGROUP_H

#include "Animal.h"
#include <string>
#include <unordered_map>
#include "nlohmann/json.hpp"

class AnimalGroup {
   private:
   static int nextId;
   int groupId;
   int animalAmount;
   std::string groupName;
   std::unordered_map<int, Animal*> animalMap;

   public: 
   AnimalGroup(const std::string& groupName);

   ~AnimalGroup();

   int getGroupId() const;

   int getAnimalAmount() const;

   std::string getGroupName() const;

   void addAnimal(Animal* animal);

   void addAnimals(std::vector<Animal*> animals);
   
   void removeAnimal(Animal* animal);

   void mergeGroup(AnimalGroup& otherGroup); // Merges other group to this group. Other group becomes empty

   nlohmann::json generateReport() const; // Generates a list of animals from the group in alphabetical order
};

#endif
