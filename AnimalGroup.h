#ifndef ANIMALGROUP_H
#define ANIMALGROUP_H

#include <string>
#include <vector>

class AnimalGroup {
   private:
   int groupID;
   std::string groupName;
   // std::vector<Animal> animals; TODO: Make animal class

   public: 
   AnimalGroup (const std::string& groupName);

   int getGroupID() const;

   std::string getGroupName() const;

};

#endif