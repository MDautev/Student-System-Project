#ifndef UI_UTILS_H
#define UI_UTILS_H

#include <iostream>
#include <string>
#include <limits>
#include "Student.h"
#include "SortStrategy.h"
#include "SortByName.h"
#include "SortByGrade.h"
#include "SortByFacultyNumber.h"

// Статичен клас за вход/изход
class UI
{
public:
    static int readInt(const std::string &prompt = "")
    {
        int value;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> value)
                break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Грешка: Моля, въведете число!\n";
        }
        return value;
    }

    static double readDouble(const std::string &prompt = "")
    {
        double value;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> value)
                break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Грешка: Моля, въведете число!\n";
        }
        return value;
    }

    static SortStrategy *chooseSortStrategy()
    {
        std::cout << "\nСортиране по: 1. Име, 2. Успех, 3. ФН, 0. Без: ";
        int choice = readInt();
        if (choice == 1)
            return new SortByName();
        if (choice == 2)
            return new SortByGrade();
        if (choice == 3)
            return new SortByFacultyNumber();
        return nullptr;
    }
};

#endif