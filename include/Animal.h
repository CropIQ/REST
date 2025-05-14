#pragma once

#include "Medicament.h"
#include <vector>
#include <utility>
#include "nlohmann/json.hpp"

class Animal {
private:
   int id;
   int groupId;
   std::string name;
   int weight; // Weight (g)
   time_t birthDate; // Time (s) since 1970
   std::pair<Medicament, int> medicaments;

public:
   Animal(const int& Id, const int& groupId, const std::string& name, const int& weight);
   Animal(const nlohmann::json& json);
   ~Animal();
   
   void recordMedicament(const Medicament& med, int amountOfMedicament); // Send information about newly recorded medicament to database

   int getId() const;
   int getGroupId() const;
   std::string getName() const;
   int getWeight() const;
   time_t getBirthDate() const;
   nlohmann::json toJson() const; // Return animal info in json format
   nlohmann::json getMedicamentHistory() const; // Return medicament history in json format

   void setGroupId(const int& groupId);
   void setName(const std::string& name);
   void setWeight(const int& weight);
   void setBirthDate(const time_t& birthDate);
   
   bool operator<(const Animal& other) const;
};

#endif