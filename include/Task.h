#pragma once

#include <string>
#include <stdexcept>

#define NOT_STARTED 0
#define IN_PROGRESS 1
#define DONE        2

#define HIGH_PRIORITY   0
#define MEDIUM_PRIORITY 1
#define LOW_PRIORITY    2

class Task {

private:
   int id;
   std::string name;
   int status;
   time_t dueDate;
   int priority;
   int assignedWorkerId;

public:
   Task(const std::string& name, const int& status, const time_t& dueDate, const int& priority);

   void setName(const std::string& name);
   void setStatus(const int& status);
   void setDueDate(const time_t& dueDate);
   void setPriority(const int& priority);

   void assignTask(const int& assignedWorkerId);

   int getId() const;
   std::string getName() const;
   time_t getDueDate() const;
   int getPriority() const;
   int getAssignedWorkerId() const;
};