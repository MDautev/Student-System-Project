#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include "People.h"

class Student : public People
{
private:
    double grades[5];
    std::string facultyNumber; // нов член

protected:
    int groupNumber;

public:
    Student(const std::string &n,
            const std::string &egn,
            const Date &d,
            double g[5],
            int groupNum,
            const std::string &fn); // нов конструктор с fn

    double calculateAverage() const;
    std::string getFacultyNumber() const;
    int getGroupNumber() const;
    void print() const;
};

#endif
