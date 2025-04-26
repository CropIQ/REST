#ifndef ANIMAL_H
#define ANIMAL_H

#include "Medicament.h"
#include <string>
#include <ctime>
#include <vector>
#include <utility>

class Animal {
   private:
   static int ID;
   int animalID;
   std::string name;
   time_t birthDate; // Time (s) since 1970
   int weight; // Weight (g)
   std::vector<std::pair<Medicament, int>> medicaments;

   public:
   Animal(const std::string& name);
   ~Animal();

   int getAnimalID() const;
   std::string getName() const;
   std::time_t getBirthDate() const;
   int getWeight() const;
   std::string getMedicamentHistory() const;

   void setName(const string& newName) { name = newName; }
   void setWeight(const int& newWeight)
   void setBirthDate(const time_t& newBirthDate)
   
   void recordMedicament(const Medicament& med, int amountOfMedicament);
};

#endif