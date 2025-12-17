#ifndef PSTUDENT_H
#define PSTUDENT_H

#include "Student.h"

class PStudent : public Student
{
public:
    PStudent(const std::string &n,
             const std::string &egn,
             const Date &d,
             Gender gender,
             double g[5],
             int groupNum,
             const std::string &fn); // нов конструктор

    bool hasScholarship() const;

    void print() const override;
};

#endif
