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
             int enroll,
             const std::string &fn);

    bool hasScholarship() const;

    void print() const override;
    People *clone() const override;
    PStudent(const PStudent &other);
    ~PStudent();
};

#endif
