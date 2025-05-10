#ifndef ANIMALGROUP_H
#define ANIMALGROUP_H

#include "Animal.h"
#include <string>
#include <unordered_map>
#include "nlohmann/json.hpp"

class AnimalGroup {
   private:
   int groupId;
   int animalAmount;
   std::string groupName;
   std::unordered_map<int, Animal*> animalMap;

   public: 
   AnimalGroup(const int& Id, const string& groupName, std::vector<Animal*> animals);

   AnimalGroup(const nlohmann::json& json);

   ~AnimalGroup();

   void addAnimal(Animal* animal); // Add single animal to group
   
   void addAnimals(std::vector<Animal*> animals); // Add multiple animals to group
   
   void removeAnimal(Animal* animal); // Remove single animal from group
   
   void mergeGroup(AnimalGroup& otherGroup); // Merges other group to this group. Other group becomes empty
   
   nlohmann::json generateReport() const; // Generates a list of animals from the group in alphabetical order
   
   int getGroupId() const;
   int getAnimalAmount() const;
   std::string getGroupName() const;

   void setGroupName(const std::string& groupName);
};

#endif
