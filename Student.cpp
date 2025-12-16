#include <iostream>
#include "Student.h"

using namespace std;

Student::Student(const string &n,
                 const string &egn,
                 const Date &d,
                 double g[5],
                 int groupNum,
                 const string &fn) // приемаме fn
    : People(n, egn, d), groupNumber(groupNum), facultyNumber(fn)
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
