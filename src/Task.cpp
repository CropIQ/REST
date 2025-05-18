#include "Task.h"

Task::Task(const std::string& name, const int& status, const time_t& dueDate, const int& priority) : 
   name(name), 
   dueDate(dueDate) 
{
   setStatus(status);
   setPriority(priority);
}

void Task::setName(const std::string& name) { this->name = name; }
void Task::setStatus(const int& status) { 
   if(status > DONE) { throw std::out_of_range("Status can't be of higher value than " + DONE + "."); }
   else { this->status = status; }
}
void Task::setDueDate(const time_t& dueDate) { this->dueDate = dueDate; }
void Task::setPriority(const int& priority) { 
   if(priority > LOW_PRIORITY) { throw std::out_of_range("Priority can't be of higher value than " + LOW_PRIORITY + "."); }
   else { this->status = status; }
}

void Task::assignTask(const int& assignedWorkerId) { this->assignedWorkerId = assignedWorkerId; }

int Task::getId() const { return id; }
std::string Task::getName() const { return name; }
time_t Task::getDueDate() const { return dueDate; }
int Task::getPriority() const { return priority; }
int Task::getAssignedWorkerId() const { return assignedWorkerId; }