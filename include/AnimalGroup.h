#ifndef ANIMALGROUP_H
#define ANIMALGROUP_H

#include "Animal.h"
#include <string>
#include <unordered_map>

class AnimalGroup {
   private:
   static int nextID;
   int groupID;
   int animalAmount;
   std::string groupName;
   std::unordered_map<int, Animal*> animalMap;

   public: 
   AnimalGroup (const std::string& groupName);

   int getGroupID() const;

   int getAnimalAmount() const;

   std::string getGroupName() const;

   void addAnimal(Animal* newAnimal);
   
   void removeAnimal(Animal* removedAnimal);

   void mergeGroup(AnimalGroup& otherGroup); // Merges this group to otherGroup. This group becomes empty

   std::string generateReport() const; // Generates a list of animals from the group in alphabetical order
};

#endif
