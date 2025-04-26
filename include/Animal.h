#ifndef ANIMAL_H
#define ANIMAL_H

#include "Medicament.h"
#include <string>
#include <ctime>
#include <vector>
#include <utility>

class Animal {
   private:
   int animalID;
   std::string name;
   time_t birthDate;
   int weight;
   std::vector<std::pair<Medicament, int>> medicaments;

   Animal(const std::string& name);
   ~Animal();

   int getAnimalID() const;
   std::string getName() const;
   std::time_t getBirthDate() const;
   int getWeight() const;
   std::string getMedicamentHistory() const;
   void recordMedicament(const Medicament& med, int amountOfMedicament);

};

#endif