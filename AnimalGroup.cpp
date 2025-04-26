#include <string>
#include <vector>
#include "AnimalGroup.h"

using std::string;

AnimalGroup::AnimalGroup (const string& groupName) : groupName(groupName) {}

int AnimalGroup::getGroupID() const { return groupID; }

string AnimalGroup::getGroupName() const { return groupName; }

