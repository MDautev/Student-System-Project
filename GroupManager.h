#ifndef GROUP_MANAGER_H
#define GROUP_MANAGER_H

#include <map>
#include "Group.h"

class GroupManager
{
private:
    std::map<int, Group> groups; // ключ = номер на група

public:
    void addStudent(int groupNumber, Student *s);
    Group *getGroup(int groupNumber);
    const std::map<int, Group> &getAllGroups() const;
    std::map<int, Group> &getAllGroups() { return groups; }
    Student *findStudentByFacultyNumber(const std::string &fn);
};

#endif
