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
#include "AppException.h"
#include "Logger.h"
#include "Config.h"
#include <vector>
#include <fstream>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;
#include <cstdlib>
#include <ctime>

// --- безопасно четене на int ---
int readInt(const std::string &prompt = "")
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
            break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Грешка: Моля, въведете число!\n";
    }
    return value;
}

// --- безопасно четене на double ---
double readDouble(const std::string &prompt = "")
{
    double value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
            break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Грешка: Моля, въведете число!\n";
    }
    return value;
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
    sortChoice = readInt();

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
    Config::load("config.ini");
    ofstream file(Config::getString("log_file"), ios::app);
    try
    {
        manager.importFromFile(Config::getString("students_file"));
    }
    catch (...)
    {
        cout << "Няма начален файл със студенти.\n";
    }
    Student *lastBackup = nullptr;
    Student *lastEditedStudent = nullptr;
    int choice;

    do
    {
        cout << "\n=== Меню ===\n";
        cout << "1. Добавяне на студент\n";
        cout << "2. Преглед на всички студенти\n";
        cout << "3. Търсене на студенти\n";
        cout << "4. Отчет на група\n";
        cout << "5. Изтриване на студент\n";
        cout << "6. Редактиране на студент\n";
        cout << "7. Върни промените\n";
        cout << "8. Преглед на лог файл\n";
        cout << "0. Изход\n";
        cout << "Изберете опция: ";
        choice = readInt();
        try
        {
            switch (choice)
            {
            case 1:
            {
                int type;
                cout << "Изберете тип студент (1 - редовен, 2 - платен): ";
                type = readInt();

                string name, egn;
                double grades[5];

                cout << "Име: ";
                getline(cin, name);
                int d = readInt("Въведете ден на раждане: ");
                int m = readInt("Въведете месец на раждане: ");
                int y = readInt("Въведете година на раждане: ");

                if (!Date::isValid(d, m, y))
                {
                    throw InvalidDateException();
                }
                Date date(d, m, y);
                char g;
                cout << "Пол (m/f): ";
                cin >> g;
                if (g != 'm' && g != 'M' && g != 'f' && g != 'F')
                {
                    throw InvalidGenderException();
                }

                Gender gender = (g == 'm' || g == 'M') ? Gender::Male : Gender::Female;

                cout << "ЕГН: ";
                cin >> egn;
                if (!People::validateEGN(egn, date, gender))
                {
                    throw InvalidEGNException();
                }
                if (manager.egnExists(egn))
                {
                    throw DuplicateEGNException();
                }

                cout << "Въведете 5 оценки: ";
                for (int i = 0; i < 5; i++)
                {
                    grades[i] = readInt();
                    if (!Student::IsGradeValid(grades[i]))
                    {
                        throw InvalidGradeException();
                    }
                }
                int groupNumber, enrollYear;
                cout << "Номер на група: ";
                groupNumber = readInt();
                if (groupNumber >= 100 || groupNumber <= 1)
                {
                    throw InvalidGroupException();
                }
                cout << "Година на записване: ";
                enrollYear = readInt();
                if (enrollYear < 2020)
                {
                    throw InvalidYearException();
                }
                // Генериране на факултетен номер
                string fn = FacultyNumberGenerator::generate(groupNumber, enrollYear);

                Student *s = nullptr;
                if (type == 1)
                    s = new Student(name, egn, date, gender, grades, groupNumber, enrollYear, fn);
                else
                    s = new PStudent(name, egn, date, gender, grades, groupNumber, enrollYear, fn);

                manager.addStudent(groupNumber, s);
                Logger::log("Добавен студент: " + name + " с факултетен номер " + fn);
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
                searchChoice = readInt();

                // === Търсене по отделни критерии ===
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
                    grp = readInt();
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
                    minGrade = readDouble();
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
                        throw StudentNotFoundException();
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
                    firstChoice = readInt();
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
                        grp = readInt();
                        firstCrit = new CriteriaByGroup(grp);
                    }
                    else if (firstChoice == 3)
                    {
                        double minGrade;
                        cout << "Минимален успех: ";
                        minGrade = readDouble();
                        firstCrit = new CriteriaByMinGrade(minGrade);
                    }

                    selectedCriteria.push_back(firstCrit);

                    int logicChoice;
                    cout << "1. AND (и)\n2. OR (или)\nИзберете логика: ";
                    logicChoice = readInt();

                    Criteria *combo = selectedCriteria[0];

                    bool adding = true;
                    while (adding)
                    {
                        cout << "Изберете допълнителен критерий или 0 за край:\n";
                        for (int i = 0; i < options.size(); i++)
                            if (!used[i])
                                cout << i + 1 << ". " << options[i] << endl;

                        int nextChoice;
                        nextChoice = readInt();
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
                            grp = readInt();
                            nextCrit = new CriteriaByGroup(grp);
                        }
                        else if (nextChoice == 3)
                        {
                            double minGrade;
                            cout << "Минимален успех: ";
                            minGrade = readDouble();
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
                int groupNum;
                cout << "Въведете номер на група: ";
                groupNum = readInt();

                auto it = manager.getAllGroups().find(groupNum);
                if (it == manager.getAllGroups().end())
                {
                    cout << "Няма такава група.\n";
                    break;
                }

                it->second.generateReport();
                break;
            }

            case 5:
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
                {
                    cout << "Студентът е изтрит успешно." << endl;
                    Logger::log("Изтрит студент с факултетен номер " + fn);
                }
                else
                    cout << "Няма такъв студент!" << endl;

                break;
            }

            case 6: // Редакция на студент
            {
                string fn;
                cout << "Въведете факултетен номер на студента за редакция: ";
                cin >> fn;

                Student *student = nullptr;

                // Търсим студента във всички групи
                for (const auto &pair : manager.getAllGroups())
                {
                    student = pair.second.findByFacultyNumber(fn);
                    if (student)
                        break;
                }

                if (!student)
                {
                    cout << "Няма такъв студент.\n";
                    break;
                }

                // Премахваме предишния backup, ако има
                if (lastBackup)
                    delete lastBackup;

                // Създаваме backup за undo
                lastBackup = dynamic_cast<Student *>(student->clone());
                lastEditedStudent = student;

                // Запазваме старите данни за лог
                double oldGrades[5];
                for (int i = 0; i < 5; i++)
                    oldGrades[i] = student->getGrades()[i];
                int oldGroup = student->getGroupNumber();
                string oldFN = student->getFacultyNumber();

                // Редакция на оценки
                cout << "Редакция на оценки (5 числа, разделени с интервал): ";
                double newGrades[5];
                for (int i = 0; i < 5; i++)
                {
                    newGrades[i] = readInt();
                    if (!Student::IsGradeValid(newGrades[i]))
                    {
                        throw InvalidGradeException();
                    }
                }
                student->setGrades(newGrades);

                // Редакция на група
                cout << "Редакция на група: ";
                int newGroup;
                newGroup = readInt();
                if (newGroup >= 100 || newGroup <= 1)
                {
                    throw InvalidGroupException();
                }
                student->setGroupNumber(newGroup);

                // Автоматична смяна на факултетен номер
                int enrollYear;
                cout << "Година на записване: ";
                enrollYear = readInt();
                if (enrollYear < 2020)
                {
                    throw InvalidYearException();
                }
                string newFN = FacultyNumberGenerator::generate(newGroup, enrollYear);
                student->setFacultyNumber(newFN);

                cout << "Промените са записани.\n";

                // Логваме старите и новите данни
                Logger::log("Редакция на студент с факултетен номер " + oldFN +
                            ": оценки [" +
                            to_string(oldGrades[0]) + ", " + to_string(oldGrades[1]) + ", " +
                            to_string(oldGrades[2]) + ", " + to_string(oldGrades[3]) + ", " +
                            to_string(oldGrades[4]) + "], група " + to_string(oldGroup) +
                            " -> нови оценки [" +
                            to_string(newGrades[0]) + ", " + to_string(newGrades[1]) + ", " +
                            to_string(newGrades[2]) + ", " + to_string(newGrades[3]) + ", " +
                            to_string(newGrades[4]) + "], група " + to_string(newGroup) +
                            ", факултетен номер " + newFN);

                break;
            }

            case 7: // Undo последна редакция
            {
                if (lastBackup && lastEditedStudent)
                {
                    // Запазваме текущите данни за лог
                    double curGrades[5];
                    for (int i = 0; i < 5; i++)
                        curGrades[i] = lastEditedStudent->getGrades()[i];
                    int curGroup = lastEditedStudent->getGroupNumber();
                    string curFN = lastEditedStudent->getFacultyNumber();

                    // Връщаме backup
                    *lastEditedStudent = *lastBackup;

                    // Логваме какво е върнато
                    Logger::log("Отменена редакция на студент с факултетен номер " + lastEditedStudent->getFacultyNumber() +
                                ": оценки [" +
                                to_string(curGrades[0]) + ", " + to_string(curGrades[1]) + ", " +
                                to_string(curGrades[2]) + ", " + to_string(curGrades[3]) + ", " +
                                to_string(curGrades[4]) + "], група " + to_string(curGroup) +
                                " -> възстановени оценки [" +
                                to_string(lastEditedStudent->getGrades()[0]) + ", " +
                                to_string(lastEditedStudent->getGrades()[1]) + ", " +
                                to_string(lastEditedStudent->getGrades()[2]) + ", " +
                                to_string(lastEditedStudent->getGrades()[3]) + ", " +
                                to_string(lastEditedStudent->getGrades()[4]) + "], група " +
                                to_string(lastEditedStudent->getGroupNumber()) +
                                ", факултетен номер " + lastEditedStudent->getFacultyNumber());

                    cout << "Последната промяна е отменена.\n";
                }
                else
                {
                    cout << "Няма промени за връщане.\n";
                }
                break;
            }
            case 8:
            {
                Logger::readLogs();
                break;
            }

            case 0:
                manager.exportToFile(Config::getString("students_file"));
                Logger::log("Запис на всички студенти и изход от програмата.");
                cout << "Изход..." << endl;
                break;

            default:
                cout << "Невалидна опция!" << endl;
            }
        }
        catch (const AppException &e)
        {
            Logger::log("Грешка: " + std::string(e.what()));
            cout << e.what() << endl;
        }
    } while (choice != 0);

    return 0;
}
