#ifndef ANIMAL_H
#define ANIMAL_H

#include "Medicament.h"
#include <string>
#include <ctime>
#include <vector>
#include <utility>
#include "nlohmann/json.hpp"

class Animal {
   private:
   static int nextId;
   int animalId;
   int groupId;
   std::string name;
   int weight; // Weight (g)
   time_t birthDate; // Time (s) since 1970
   std::pair<Medicament, int> medicaments;

   public:
   Animal(const std::string& name);
   ~Animal();

   int getAnimalId() const;
   int getGroupId() const;
   std::string getName() const;
   int getWeight() const;
   std::time_t getBirthDate() const;
   nlohmann::json getAnimalInfo() const; // Return animal info in json format
   nlohmann::json getMedicamentHistory() const; // Return medicament history in json format

   void setGroupId(const int& groupId);
   void setName(const string& name);
   void setWeight(const int& weight);
   void setBirthDate(const time_t& birthDate);
   
   void recordMedicament(const Medicament& med, int amountOfMedicament);
   bool operator<(const Animal& other) const;
};

#endif