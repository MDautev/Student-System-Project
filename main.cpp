#include <iostream>
#include "GroupManager.h"
#include "Student.h"
#include "PStudent.h"
#include "Date.h"
#include "FacultyNumberGenerator.h"
#include "CriteriaByName.h"
#include "CriteriaByGroup.h"
#include "CriteriaByMinGrade.h"
#include "AndCriteria.h"
#include "OrCriteria.h"
#include "SortByFacultyNumber.h"
#include "SortByGrade.h"
#include "SortByName.h"
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

#include <cstdlib>
#include <ctime>

// Временно за тестове
void generateTestStudents(GroupManager &manager, int count = 10)
{
    srand(static_cast<unsigned>(time(0)));

    string names[] = {"Иван", "Георги", "Мария", "Елена", "Димитър", "Петя", "Александър", "Светла"};
    int groupNumbers[] = {11, 12, 21, 22};
    int enrollYears[] = {2021, 2022, 2023};

    for (int i = 0; i < count; i++)
    {
        string name = names[rand() % 8];
        string egn = to_string(9000000000 + rand() % 1000000000); // случаен ЕГН
        Date birthDate(rand() % 28 + 1, rand() % 12 + 1, 1995 + rand() % 10);

        double grades[5];
        for (int j = 0; j < 5; j++)
            grades[j] = 3.0 + static_cast<double>(rand() % 21) / 10.0; // 3.0 - 5.0

        int group = groupNumbers[rand() % 4];
        int year = enrollYears[rand() % 3];

        string fn = FacultyNumberGenerator::generate(group, year);

        Student *s = new Student(name, egn, birthDate, grades, group, fn);
        manager.addStudent(group, s);
    }
}

SortStrategy *chooseSortStrategy()
{
    int sortChoice;
    cout << "\nСортиране по:\n";
    cout << "1. Име\n";
    cout << "2. Успех\n";
    cout << "3. Факултетен номер\n";
    cout << "0. Без сортиране\n";
    cout << "Избор: ";
    cin >> sortChoice;

    switch (sortChoice)
    {
    case 1:
        return new SortByName();
    case 2:
        return new SortByGrade();
    case 3:
        return new SortByFacultyNumber();
    default:
        return nullptr;
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001); // UTF-8 за изход
    SetConsoleCP(65001);       // UTF-8 за въвеждане
#endif

    GroupManager manager;

    // генерираме 10 тестови студенти
    generateTestStudents(manager, 10);

    int choice;

    do
    {
        cout << "\n=== Меню ===\n";
        cout << "1. Добавяне на студент\n";
        cout << "2. Преглед на всички студенти\n";
        cout << "3. Търсене на студенти\n";
        cout << "4. Изтриване на студент\n";
        cout << "0. Изход\n";
        cout << "Изберете опция: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int type;
            cout << "Изберете тип студент (1 - редовен, 2 - платен): ";
            cin >> type;
            cin.ignore();

            string name, egn;
            int d, m, y;
            double grades[5];

            cout << "Име: ";
            getline(cin, name);
            cout << "ЕГН: ";
            cin >> egn;
            cout << "Рожден ден (ден месец година): ";
            cin >> d >> m >> y;
            cout << "Въведете 5 оценки: ";
            for (int i = 0; i < 5; i++)
                cin >> grades[i];

            int groupNumber, enrollYear;
            cout << "Номер на група (2 цифри): ";
            cin >> groupNumber;
            cout << "Година на записване: ";
            cin >> enrollYear;

            // Генериране на факултетен номер
            string fn = FacultyNumberGenerator::generate(groupNumber, enrollYear);

            Date date(d, m, y);

            Student *s = nullptr;
            if (type == 1)
                s = new Student(name, egn, date, grades, groupNumber, fn);
            else
                s = new PStudent(name, egn, date, grades, groupNumber, fn);

            manager.addStudent(groupNumber, s);

            break;
        }

        case 2:
        {
            cout << "\n=== Всички студенти ===" << endl;

            SortStrategy *strategy = chooseSortStrategy();

            for (auto &pair : manager.getAllGroups())
            {
                cout << "=== Група " << pair.first << " ===" << endl;

                if (strategy)
                {
                    pair.second.setSortStrategy(strategy);
                    pair.second.sortStudents();
                }

                pair.second.printAll();
            }

            delete strategy;
            break;
        }

        case 3:
        {
            vector<Student *> results;
            SortStrategy *strategy = nullptr;

            int searchChoice;
            cout << "\n=== Търсене на студенти ===\n";
            cout << "1. По име\n";
            cout << "2. По група\n";
            cout << "3. По минимален успех\n";
            cout << "4. По факултетен номер\n";
            cout << "5. По няколко критерия (AND / OR)\n";
            cout << "Изберете опция: ";
            cin >> searchChoice;

            // === По отделни критерии ===
            if (searchChoice == 1)
            {
                string name;
                cout << "Въведете име или част от име: ";
                cin.ignore();
                getline(cin, name);

                CriteriaByName c(name);
                for (const auto &pair : manager.getAllGroups())
                    for (int i = 0; i < pair.second.getCount(); i++)
                    {
                        Student *s = pair.second.getStudentAt(i);
                        if (c.meetsCriteria(*s))
                            results.push_back(s);
                    }
            }
            else if (searchChoice == 2)
            {
                int grp;
                cout << "Номер на група: ";
                cin >> grp;
                CriteriaByGroup c(grp);
                for (const auto &pair : manager.getAllGroups())
                    for (int i = 0; i < pair.second.getCount(); i++)
                    {
                        Student *s = pair.second.getStudentAt(i);
                        if (c.meetsCriteria(*s))
                            results.push_back(s);
                    }
            }
            else if (searchChoice == 3)
            {
                double minGrade;
                cout << "Минимален успех: ";
                cin >> minGrade;
                CriteriaByMinGrade c(minGrade);
                for (const auto &pair : manager.getAllGroups())
                    for (int i = 0; i < pair.second.getCount(); i++)
                    {
                        Student *s = pair.second.getStudentAt(i);
                        if (c.meetsCriteria(*s))
                            results.push_back(s);
                    }
            }
            else if (searchChoice == 4)
            {
                string fn;
                cout << "Факултетен номер: ";
                cin >> fn;
                for (const auto &pair : manager.getAllGroups())
                {
                    Student *s = pair.second.findByFacultyNumber(fn);
                    if (s)
                    {
                        results.push_back(s);
                        break;
                    }
                }
                if (results.empty())
                    cout << "Няма такъв студент.\n";
            }
            else if (searchChoice == 5)
            {
                vector<Criteria *> selectedCriteria;
                vector<string> options = {"Име", "Група", "Минимален успех"};
                vector<bool> used(options.size(), false);

                int firstChoice;
                cout << "Изберете първи критерий:\n";
                for (int i = 0; i < options.size(); i++)
                    cout << i + 1 << ". " << options[i] << endl;
                cin >> firstChoice;
                used[firstChoice - 1] = true;

                Criteria *firstCrit = nullptr;
                if (firstChoice == 1)
                {
                    string name;
                    cout << "Име или част от име: ";
                    cin.ignore();
                    getline(cin, name);
                    firstCrit = new CriteriaByName(name);
                }
                else if (firstChoice == 2)
                {
                    int grp;
                    cout << "Номер на група: ";
                    cin >> grp;
                    firstCrit = new CriteriaByGroup(grp);
                }
                else if (firstChoice == 3)
                {
                    double minGrade;
                    cout << "Минимален успех: ";
                    cin >> minGrade;
                    firstCrit = new CriteriaByMinGrade(minGrade);
                }

                selectedCriteria.push_back(firstCrit);

                int logicChoice;
                cout << "1. AND (и)\n2. OR (или)\nИзберете логика: ";
                cin >> logicChoice;

                Criteria *combo = selectedCriteria[0];

                bool adding = true;
                while (adding)
                {
                    cout << "Изберете допълнителен критерий или 0 за край:\n";
                    for (int i = 0; i < options.size(); i++)
                        if (!used[i])
                            cout << i + 1 << ". " << options[i] << endl;

                    int nextChoice;
                    cin >> nextChoice;
                    if (nextChoice == 0)
                        break;
                    used[nextChoice - 1] = true;

                    Criteria *nextCrit = nullptr;
                    if (nextChoice == 1)
                    {
                        string name;
                        cout << "Име или част от име: ";
                        cin.ignore();
                        getline(cin, name);
                        nextCrit = new CriteriaByName(name);
                    }
                    else if (nextChoice == 2)
                    {
                        int grp;
                        cout << "Номер на група: ";
                        cin >> grp;
                        nextCrit = new CriteriaByGroup(grp);
                    }
                    else if (nextChoice == 3)
                    {
                        double minGrade;
                        cout << "Минимален успех: ";
                        cin >> minGrade;
                        nextCrit = new CriteriaByMinGrade(minGrade);
                    }

                    if (logicChoice == 1)
                        combo = new AndCriteria(*combo, *nextCrit);
                    else
                        combo = new OrCriteria(*combo, *nextCrit);

                    selectedCriteria.push_back(nextCrit);
                }

                for (const auto &pair : manager.getAllGroups())
                    for (int i = 0; i < pair.second.getCount(); i++)
                    {
                        Student *s = pair.second.getStudentAt(i);
                        if (combo->meetsCriteria(*s))
                            results.push_back(s);
                    }

                for (auto c : selectedCriteria)
                    delete c;
                delete combo;
            }

            // === Сортиране и печат ===
            if (!results.empty())
            {
                if (searchChoice != 4)
                {
                    strategy = chooseSortStrategy();

                    if (strategy)
                    {
                        sort(results.begin(), results.end(),
                             [strategy](Student *a, Student *b)
                             {
                                 return strategy->compare(a, b);
                             });
                    }
                }
                for (Student *s : results)
                {
                    cout << "------------------\n";
                    s->print();
                }
            }

            delete strategy;
            break;
        }

        case 4:
        {
            string fn;
            cout << "Въведете факултетен номер за изтриване: ";
            cin >> fn;

            bool deleted = false;
            for (auto &pair : manager.getAllGroups())
            {
                if (pair.second.removeByFacultyNumber(fn))
                {
                    deleted = true;
                    break;
                }
            }

            if (deleted)
                cout << "Студентът е изтрит успешно." << endl;
            else
                cout << "Няма такъв студент!" << endl;

            break;
        }

        case 0:
            cout << "Изход..." << endl;
            break;

        default:
            cout << "Невалидна опция!" << endl;
        }
    } while (choice != 0);

    return 0;
}
