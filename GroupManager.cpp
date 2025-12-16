#include "GroupManager.h"

void GroupManager::addStudent(int groupNumber, Student *s)
{
    groups[groupNumber].addStudent(s);
}

Group *GroupManager::getGroup(int groupNumber)
{
    auto it = groups.find(groupNumber);
    if (it != groups.end())
        return &it->second;
    return nullptr;
}

const std::map<int, Group> &GroupManager::getAllGroups() const
{
    return groups;
}
