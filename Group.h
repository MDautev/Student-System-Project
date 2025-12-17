#ifndef GROUP_H
#define GROUP_H

#include "Student.h"
#include "SortStrategy.h"

class Group
{
private:
    Student *students[30];
    int count;
    SortStrategy *sortStrategy; // ⬅️ НОВО

public:
    Group();

    void addStudent(Student *s);
    void printAll() const;

    int getCount() const { return count; }
    Student *getStudentAt(int index) const { return students[index]; }
    Student *findByFacultyNumber(const std::string &fn) const;
    bool removeByFacultyNumber(const std::string &fn);
    void generateReport() const;

    void setSortStrategy(SortStrategy *strategy);
    void sortStudents();
};

#endif
