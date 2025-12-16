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
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001); // UTF-8 за изход
    SetConsoleCP(65001);       // UTF-8 за въвеждане
#endif

    GroupManager manager;
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
            for (const auto &pair : manager.getAllGroups())
            {
                cout << "=== Група " << pair.first << " ===" << endl;
                pair.second.printAll();
            }
            break;
        }

        case 3:
        {
            int searchChoice;
            cout << "\n=== Търсене на студенти ===\n";
            cout << "1. По име\n";
            cout << "2. По група\n";
            cout << "3. По минимален успех\n";
            cout << "4. По факултетен номер\n";
            cout << "5. По няколко критерия (AND / OR)\n";
            cout << "Изберете опция: ";
            cin >> searchChoice;

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
                            s->print();
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
                            s->print();
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
                            s->print();
                    }
            }
            else if (searchChoice == 4)
            {
                string fn;
                cout << "Факултетен номер: ";
                cin >> fn;

                bool found = false;
                for (const auto &pair : manager.getAllGroups())
                {
                    Student *s = pair.second.findByFacultyNumber(fn);
                    if (s)
                    {
                        s->print();
                        found = true;
                        break;
                    }
                }

                if (!found)
                    cout << "Няма такъв студент.\n";
            }
            else if (searchChoice == 5)
            {
                vector<Criteria *> selectedCriteria;
                vector<string> options = {"Име", "Група", "Минимален успех"}; // Факулт. номер не се слага тук
                vector<bool> used(options.size(), false);

                // Избор на първи критерий
                int firstChoice;
                cout << "Изберете първи критерий:\n";
                for (int i = 0; i < options.size(); i++)
                    cout << i + 1 << ". " << options[i] << endl;
                cin >> firstChoice;
                used[firstChoice - 1] = true;

                Criteria *firstCrit = nullptr;
                if (firstChoice == 1) // Име
                {
                    string name;
                    cout << "Име или част от име: ";
                    cin.ignore();
                    getline(cin, name);
                    firstCrit = new CriteriaByName(name);
                }
                else if (firstChoice == 2) // Група
                {
                    int grp;
                    cout << "Номер на група: ";
                    cin >> grp;
                    firstCrit = new CriteriaByGroup(grp);
                }
                else if (firstChoice == 3) // Минимален успех
                {
                    double minGrade;
                    cout << "Минимален успех: ";
                    cin >> minGrade;
                    firstCrit = new CriteriaByMinGrade(minGrade);
                }

                selectedCriteria.push_back(firstCrit);

                // Логика AND / OR
                int logicChoice;
                cout << "1. AND (и)\n2. OR (или)\nИзберете логика: ";
                cin >> logicChoice;

                Criteria *combo = selectedCriteria[0];

                // Добавяне на останалите опции
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

                    // Комбиниране
                    if (logicChoice == 1)
                        combo = new AndCriteria(*combo, *nextCrit);
                    else
                        combo = new OrCriteria(*combo, *nextCrit);

                    selectedCriteria.push_back(nextCrit);
                }

                // Изпълнение на търсенето
                for (const auto &pair : manager.getAllGroups())
                    for (int i = 0; i < pair.second.getCount(); i++)
                    {
                        Student *s = pair.second.getStudentAt(i);
                        if (combo->meetsCriteria(*s))
                            s->print();
                    }

                // Почистване
                for (auto c : selectedCriteria)
                    delete c;
                delete combo;
            }

            break;
        }

        case 4:
        {
            string fn;
            cout << "Въведете факултетен номер за изтриване: ";
            cin >> fn;

            bool deleted = false;
            for (auto &pair : manager.getAllGroups()) // тук ще се вика неконстантната getAllGroups()
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
