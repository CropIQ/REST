#pragma once

#include "Medicament.h"
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>

class Animal {
private:
   int animalId;
   int groupId;
   std::string name;
   int weight; // Weight (g)
   std::string birthDateTime; // Saves datestamp in format YYYY-MM-DD HH:MM:SS.ffffff for database
   std::vector<std::pair<std::pair<int, int>, std::string>> medicaments; //storing medicament as {{medicamentId, amount}, date}, date as string

public:
   Animal(const int& Id, const int& groupId, const std::string& name, const int& weight);
   Animal(const nlohmann::json& json);
   ~Animal();
   
   void recordMedicament(const Medicament& med, int amountOfMedicament); // Send information about newly recorded medicament to database

   int getAnimalId() const;
   int getGroupId() const;
   std::string getName() const;
   int getWeight() const;
   std::string getBirthDateTime() const;
   nlohmann::json toJson() const; // Return animal info in json format
   nlohmann::json getMedicamentHistory() const; // Return medicament history in json format

   void setGroupId(const int& groupId);
   void setName(const std::string& name);
   void setWeight(const int& weight);
   void setBirthDate(const std::string& birthDate);
   
   bool operator<(const Animal& other) const;
};