#include <iostream>
#include "Student.h"

using namespace std;

// Copy constructor за deep copy
Student::Student(const Student &other)
    : People(other.name, other.EGN, other.birthDate, other.gender), // базов конструктор
      groupNumber(other.groupNumber),
      facultyNumber(other.facultyNumber)
{
    // копираме оценките
    for (int i = 0; i < 5; i++)
        grades[i] = other.grades[i];
}

Student::Student(const string &n,
                 const string &egn,
                 const Date &d,
                 Gender gender,
                 double g[5],
                 int groupNum,
                 const string &fn) // приемаме fn
    : People(n, egn, d, gender), groupNumber(groupNum), facultyNumber(fn)
{
    for (int i = 0; i < 5; i++)
    {
        grades[i] = g[i];
    }
}

double Student::calculateAverage() const
{
    double sum = 0;
    for (int i = 0; i < 5; i++)
        sum += grades[i];
    return sum / 5;
}

string Student::getFacultyNumber() const
{
    return facultyNumber;
}

int Student::getGroupNumber() const
{
    return groupNumber;
}

void Student::print() const
{
    People::print();
    cout << "Фак. номер: " << facultyNumber
         << ", Среден успех: " << calculateAverage()
         << ", Възраст: " << getAge() << " години" << endl;
}

People *Student::clone() const
{
    return new Student(*this); // използваме copy constructor за deep copy
}

void Student::setGrades(const double newGrades[5])
{
    for (int i = 0; i < 5; i++)
        grades[i] = newGrades[i];
}

void Student::setGroupNumber(int newGroupNumber)
{
    groupNumber = newGroupNumber;
}

Student::~Student() {}
