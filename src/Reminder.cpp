#include "Reminder.h"

int Reminder::categoryCounter = 0;

Reminder::Reminder(const std::string& name, const int& dueDate, const int& repeatInterval, const int& whichCategory) : 
   name(name),
   dueDate(dueDate),
   repeatInterval(repeatInterval)
{
   auto it = categories.find(whichCategory);
   if (it != categories.end()) {
      category.first = it->first;
      category.second = it->second;
   } else {
      throw std::invalid_argument("Invalid category ID: " + std::to_string(whichCategory));
   }
}

Reminder::Reminder(const Reminder& other) {
   this->name = other.name;
   this->dueDate = other.dueDate;
   this->repeatInterval = other.repeatInterval;
   this->category = other.category;
}

Reminder::~Reminder() {
   delete assignedWorkerId;
   delete linkedAnimalId;
}

void Reminder::setName(const std::string& name) { this->name = name; }
void Reminder::setDueDate(const int& dueDate) { this->dueDate = dueDate; }
void Reminder::setRepeatInterval(const int& repeatInterval) { this->repeatInterval = repeatInterval; }
void Reminder::setCategory(const int& which) {
   auto it = categories.find(which);
   if (it != categories.end()) {
      category.first = it->first;
      category.second = it->second;
   } else {
      throw std::invalid_argument("Invalid category ID: " + std::to_string(which));
   }  
}
void Reminder::setAssignedWorkerId(const int& id) { *assignedWorkerId = id; }
void Reminder::setLinkedAnimalId(const int& id) { *linkedAnimalId = id; }

void Reminder::createCategory(const std::string& categoryName) { categories[++categoryCounter] = categoryName; }
void Reminder::deleteCategory(const int& which) { categories.erase(which); }

nlohmann::json Reminder::toJson() const {
   return {
      {"id", id},
      {"dueDate", dueDate},
      {"repeatInterval", repeatInterval},
      {"category", category.second},
      {"categoryId", category.first},
      {"assignedWorkerId", *assignedWorkerId},
      {"linkedAnimalId", *linkedAnimalId}
   };
}