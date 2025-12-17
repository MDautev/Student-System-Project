#include <iostream>
#include "PStudent.h"

using namespace std;

PStudent::PStudent(const PStudent &other)
    : Student(other) // извиква copy constructor на Student
{
    // Няма нужда от нов масив, Student вече го копира
}

PStudent::PStudent(const std::string &n,
                   const std::string &egn,
                   const Date &d,
                   Gender gender,
                   double g[5],
                   int gn,
                   const std::string &fn)
    : Student(n, egn, d, gender, g, gn, fn) {}

bool PStudent::hasScholarship() const
{
    return calculateAverage() > 4.5;
}

void PStudent::print() const
{
    Student::print();
    cout << "Право на стипендия: " << (hasScholarship() ? "Да" : "Не") << endl;
}

People *PStudent::clone() const
{
    return new PStudent(*this);
}

PStudent::~PStudent() {}