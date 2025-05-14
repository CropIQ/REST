#pragma once
#include <string>
#include <map>
#include <utility>
#include <nlohmann/json.hpp>

class Reminder {
private:
   static std::map<int, std::string> categories;
   static int categoryCounter;

   int id;
   std::string name;
   int dueDate; // maybe change to time_t
   int repeatInterval;
   std::pair<int, std::string> category;
   int *assignedWorkerId;
   int *linkedAnimalId;

public:  

   Reminder(const std::string& name, const int& dueDate, const int& repeatInterval, const int& whichCategory);
   Reminder(const Reminder& other);
   ~Reminder();

   void setName(const std::string& name);
   void setDueDate(const int& dueDate);
   void setRepeatInterval(const int& repeatInterval);
   void setCategory(const int& which);
   void setAssignedWorkerId(const int& id);
   void setLinkedAnimalId(const int& id);

   void createCategory(const std::string& categoryName);
   void deleteCategory(const int& which);

   void triggerReminder(); // TODO: figure out how to trigget the reminder

   nlohmann::json toJson() const;
};