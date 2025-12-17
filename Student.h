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
            Gender gender,
            double g[5],
            int groupNum,
            const std::string &fn); // нов конструктор с fn

    double calculateAverage() const;
    std::string getFacultyNumber() const;
    int getGroupNumber() const;
    static bool IsGradeValid(const float grade);
    void print() const;
    void setGrades(const double newGrades[5]);
    void setGroupNumber(int newGroupNumber);
    People *clone() const override;
    Student(const Student &other); // copy constructor
    void setFacultyNumber(const std::string &fn) { facultyNumber = fn; }
    ~Student();
};

#endif
