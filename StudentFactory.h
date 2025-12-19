#ifndef STUDENT_FACTORY_H
#define STUDENT_FACTORY_H

#include "Student.h"
#include "PStudent.h"
#include "UIUtils.h" // Увери се, че това е името на файла с класа UI
#include "FacultyNumberGenerator.h"
#include "AppException.h"
#include "Date.h"

class StudentFactory
{
public:
    static Student *createFromConsole(GroupManager &manager)
    {
        int type = UI::readInt("Тип (1-редовен, 2-платен): ");
        std::string name, egn;
        std::cout << "Име: ";
        std::cin.ignore();
        std::getline(std::cin, name);

        int d = UI::readInt("Ден: ");
        int m = UI::readInt("Месец: ");
        int y = UI::readInt("Година: ");

        if (!Date::isValid(d, m, y))
            throw InvalidDateException();
        Date date(d, m, y);

        char g;
        std::cout << "Пол (m/f): ";
        std::cin >> g;
        Gender gender = (g == 'm' || g == 'M') ? Gender::Male : Gender::Female;

        std::cout << "ЕГН: ";
        std::cin >> egn;
        if (!People::validateEGN(egn, date, gender))
            throw InvalidEGNException();
        if (manager.egnExists(egn)) throw DuplicateEGNException();

        double grades[5];
        std::cout << "Въведете 5 оценки: ";
        for (int i = 0; i < 5; i++)
        {
            grades[i] = UI::readDouble();
            if (!Student::IsGradeValid(grades[i]))
                throw InvalidGradeException();
        }

        int group = UI::readInt("Номер на група: ");
        if (group >= 100 || group <= 1)
            throw InvalidGroupException();
        int year = UI::readInt("Година на записване: ");
        if (year < 1900 || year > 2025) throw InvalidYearException();
        

        std::string fn = FacultyNumberGenerator::generate(group, year);
        if (type == 1)
            return new Student(name, egn, date, gender, grades, group, year, fn);
        else
            return new PStudent(name, egn, date, gender, grades, group, year, fn);
    }
};
#endif