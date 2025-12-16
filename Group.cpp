#include <iostream>
#include "Group.h"

using namespace std;

Group::Group()
{
    count = 0;
}

void Group::addStudent(Student *s)
{
    if (count < 30)
    {
        students[count++] = s;
    }
    else
    {
        cout << "Групата е пълна!" << endl;
    }
}

void Group::printAll() const
{
    for (int i = 0; i < count; i++)
    {
        cout << "------------------" << endl;
        students[i]->print();
    }
}

Student *Group::findByFacultyNumber(const string &fn) const
{
    for (int i = 0; i < count; i++)
    {
        if (students[i]->getFacultyNumber() == fn)
            return students[i];
    }
    return nullptr;
}

bool Group::removeByFacultyNumber(const string &fn)
{
    for (int i = 0; i < count; i++)
    {
        if (students[i]->getFacultyNumber() == fn)
        {
            delete students[i]; // освобождаваме паметта

            // изместваме останалите студенти
            for (int j = i; j < count - 1; j++)
            {
                students[j] = students[j + 1];
            }
            count--;
            return true; // успешно изтрит
        }
    }
    return false; // няма такъв студент
}
