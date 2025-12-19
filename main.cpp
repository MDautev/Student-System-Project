#include <iostream>
#include "GroupManager.h"
#include "Config.h"
#include "Logger.h"
#include "AppException.h"
#include "UIUtils.h"
#include "StudentFactory.h"
#include "SearchManager.h"
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void handleEdit(GroupManager &mgr, Student *&backup, Student *&edited)
{
    string fn;
    cout << "Въведете ФН за редакция: ";
    cin >> fn;
    edited = mgr.findStudentByFacultyNumber(fn);
    if (!edited)
        throw StudentNotFoundException();

    if (backup)
        delete backup;
    backup = dynamic_cast<Student *>(edited->clone());

    double grades[5];
    cout << "Нови 5 оценки: ";
    for (int i = 0; i < 5; i++)
    {
        grades[i] = UI::readDouble();
        if (!Student::IsGradeValid(grades[i]))
            throw InvalidGradeException();
    }
    edited->setGrades(grades);
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    GroupManager manager;
    Config::load("config.ini");
    try
    {
        manager.importFromFile(Config::getString("students_file"));
    }
    catch (...)
    {
    }

    Student *lastBackup = nullptr, *lastEdited = nullptr;
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
        choice = UI::readInt();
        try
        {
            switch (choice)
            {
            case 1:
            {
                Student *s = StudentFactory::createFromConsole(manager);
                manager.addStudent(s->getGroupNumber(), s);
                Logger::log("Добавен студент: " + s->getName());
                break;
            }
            case 2:
            {
                SortStrategy *st = UI::chooseSortStrategy();
                for (auto &p : manager.getAllGroups())
                {
                    if (st)
                    {
                        p.second.setSortStrategy(st);
                        p.second.sortStudents();
                    }
                    p.second.printAll();
                }
                delete st;
                break;
            }
            case 3:
            { // Търсене с последващо сортиране
                auto res = SearchManager::performSearch(manager);

                if (!res.empty())
                {
                    // Потребителят избира как да се подредят намерените резултати
                    SortStrategy *st = UI::chooseSortStrategy();

                    if (st)
                    {
                        // Използване на std::sort с ламбда функция и избраната стратегия
                        std::sort(res.begin(), res.end(), [st](Student *a, Student *b)
                                  { return st->compare(a, b); });
                    }

                    // Извеждане на сортираните резултати
                    for (size_t i = 0; i < res.size(); i++)
                    {
                        cout << "------------------\n";
                        res[i]->print();
                    }

                    delete st; // Освобождаване на заделената памет за стратегията
                }
                else
                {
                    cout << "Няма намерени студенти по тези критерии.\n";
                }
                break;
            }
                // ... в main цикъла switch(choice) ...
            case 4:
            { // Отчет на група
                int gNum = UI::readInt("Номер на група: ");
                Group *g = manager.getGroup(gNum);
                if (g)
                    g->generateReport();
                else
                    std::cout << "Групата не съществува!\n";
                break;
            }
            case 5:
            { // ИЗТРИВАНЕ - Възстановено
                std::string fn;
                std::cout << "ФН за изтриване: ";
                std::cin >> fn;
                bool deleted = false;
                // Обхождаме всички групи, за да намерим и изтрием студента
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
                    std::cout << "Студентът е изтрит.\n";
                    Logger::log("Изтрит студент с ФН: " + fn);
                }
                else
                {
                    std::cout << "Студентът не е намерен!\n";
                }
                break;
            }
                // ... останалите случаи ...
            case 6:
                handleEdit(manager, lastBackup, lastEdited);
                Logger::log("Редактиран студент с ФН: " + lastEdited->getFacultyNumber());
                break;
            case 7:
                if (lastBackup && lastEdited)
                    *lastEdited = *lastBackup;
                     Logger::log("Undo: Възстановени данни за студент: " + lastEdited->getName());
                break;
            case 8:
                Logger::readLogs();
                break;
            case 0:
                manager.exportToFile(Config::getString("students_file"));
                break;
            }
        }
        catch (const AppException &e)
        {
            cout << e.what() << endl;
            Logger::log("Грешка: " + string(e.what()));
        }
    } while (choice != 0);
    delete lastBackup;
    return 0;
}