#ifndef ANIMALGROUP_H
#define ANIMALGROUP_H

#include <string>
#include <unordered_map>

class AnimalGroup {
   private:
   static int nextID;
   int groupID;
   int animalAmount;
   std::string groupName;
   std::unordered_map<int, Animal> animalMap;

   public: 
   AnimalGroup (const std::string& groupName);

   int getGroupID() const;

   int getAnimalAmount() const;

   std::string getGroupName() const;

};

#endif