#include <iostream>
#include "Group.h"
#include "PStudent.h"
#include <limits>
#include <iomanip>

using namespace std;

Group::Group()
{
    count = 0;
    sortStrategy = nullptr;
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

void Group::setSortStrategy(SortStrategy *strategy)
{
    sortStrategy = strategy;
}

void Group::sortStudents()
{
    if (!sortStrategy)
        return;

    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (!sortStrategy->compare(students[i], students[j]))
            {
                Student *tmp = students[i];
                students[i] = students[j];
                students[j] = tmp;
            }
}

void Group::generateReport() const
{
    if (count == 0)
    {
        cout << "Групата е празна.\n";
        return;
    }

    int totalStudents = count;
    double sumAverage = 0.0;

    double minAvg = numeric_limits<double>::max();
    double maxAvg = numeric_limits<double>::lowest();

    int scholarshipCount = 0;

    // разпределение по успех
    int under3 = 0;
    int from3to4 = 0;
    int from4to5 = 0;
    int from5to6 = 0;

    for (int i = 0; i < count; i++)
    {
        double avg = students[i]->calculateAverage();
        sumAverage += avg;

        if (avg < minAvg)
            minAvg = avg;
        if (avg > maxAvg)
            maxAvg = avg;

        // стипендия (само PStudent)
        if (PStudent *ps = dynamic_cast<PStudent *>(students[i]))
        {
            if (ps->hasScholarship())
                scholarshipCount++;
        }

        // разпределение
        if (avg < 3.0)
            under3++;
        else if (avg < 4.0)
            from3to4++;
        else if (avg < 5.0)
            from4to5++;
        else
            from5to6++;
    }

    double groupAverage = sumAverage / totalStudents;

    // === Печат ===
    cout << "\n=== Статистически отчет за група ===\n";
    cout << "Брой студенти: " << totalStudents << endl;
    cout << fixed << setprecision(2);
    cout << "Среден успех на групата: " << groupAverage << endl;
    cout << "Студенти със стипендия: " << scholarshipCount << endl;
    cout << "Най-нисък успех: " << minAvg << endl;
    cout << "Най-висок успех: " << maxAvg << endl;

    cout << "\n--- Разпределение по успех ---\n";
    cout << "< 3.00      : " << under3 << endl;
    cout << "3.00 - 3.99 : " << from3to4 << endl;
    cout << "4.00 - 4.99 : " << from4to5 << endl;
    cout << "5.00 - 6.00 : " << from5to6 << endl;
}