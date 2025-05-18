#include "TaskClass.h"

Task::Task(const std::string& name, const int& status, const time_t& dueDate, const int& priority) : 
   name(name), 
   dueDate(dueDate) 
{
   setStatus(status);
   setPriority(priority);
}

Task::Task(const nlohmann::json& json) {
   id = json.at("id").get<int>();
   name = json.at("name").get<std::string>();
   status = json.at("status").get<int>();
   dueDate = json.at("dueDate").get<time_t>();
   priority = json.at("priority").get<int>();
   assignedWorkerId = json.at("assignedWorkerId").get<int>();
}

void Task::setName(const std::string& name) { this->name = name; }
void Task::setStatus(const int& status) { 
   if(status > DONE) { throw std::out_of_range("Status can't be of higher value than 2."); }
   else { this->status = status; }
}
void Task::setDueDate(const time_t& dueDate) { this->dueDate = dueDate; }
void Task::setPriority(const int& priority) { 
   if(priority > LOW_PRIORITY) { throw std::out_of_range("Priority can't be of higher value than 2."); }
   else { this->status = status; }
}

void Task::assignTask(const int& assignedWorkerId) { this->assignedWorkerId = assignedWorkerId; }

int Task::getId() const { return id; }
std::string Task::getName() const { return name; }
time_t Task::getDueDate() const { return dueDate; }
int Task::getPriority() const { return priority; }
int Task::getAssignedWorkerId() const { return assignedWorkerId; }

nlohmann::json Task::toJson() const {
   return {
      {"id", id},
      {"name", name},
      {"status", status},
      {"dueDate", dueDate},
      {"priority", priority},
      {"assignedWorkerId", assignedWorkerId} 
   };
}