#ifndef SEARCH_MANAGER_H
#define SEARCH_MANAGER_H

#include <vector>
#include <string>
#include "GroupManager.h"
#include "CriteriaByName.h"
#include "CriteriaByGroup.h"
#include "CriteriaByMinGrade.h"
#include "AndCriteria.h"
#include "OrCriteria.h"
#include "UIUtils.h"

class SearchManager
{
public:
    static std::vector<Student *> performSearch(GroupManager &manager)
    {
        std::vector<Student *> results;
        std::cout << "\n1.По име 2.По група 3.По успех 4.По ФН 5.Комбинирано (AND/OR)\n";
        int choice = UI::readInt("Избор: ");

        if (choice == 4)
        {
            std::string fn;
            std::cout << "Въведете ФН: ";
            std::cin >> fn;
            Student *s = manager.findStudentByFacultyNumber(fn);
            if (s)
                results.push_back(s);
            return results;
        }

        Criteria *finalCriteria = nullptr;

        if (choice == 5)
        {
            finalCriteria = buildComplexCriteria();
        }
        else
        {
            finalCriteria = createSingleCriteria(choice);
        }

        if (finalCriteria)
        {
            for (auto const &pair : manager.getAllGroups())
            {
                const Group &g = pair.second;
                for (int i = 0; i < g.getCount(); i++)
                {
                    if (finalCriteria->meetsCriteria(*g.getStudentAt(i)))
                        results.push_back(g.getStudentAt(i));
                }
            }
            delete finalCriteria;
        }
        return results;
    }

private:
    static Criteria *createSingleCriteria(int choice)
    {
        if (choice == 1)
        {
            std::string n;
            std::cout << "Име: ";
            std::cin.ignore();
            std::getline(std::cin, n);
            return new CriteriaByName(n);
        }
        else if (choice == 2)
        {
            return new CriteriaByGroup(UI::readInt("Група: "));
        }
        else if (choice == 3)
        {
            return new CriteriaByMinGrade(UI::readDouble("Мин. успех: "));
        }
        return nullptr;
    }

    static Criteria *buildComplexCriteria()
    {
        std::cout << "Първи критерий (1.Име 2.Група 3.Успех): ";
        Criteria *c1 = createSingleCriteria(UI::readInt());
        int logic = UI::readInt("Логика (1.AND 2.OR): ");
        std::cout << "Втори критерий (1.Име 2.Група 3.Успех): ";
        Criteria *c2 = createSingleCriteria(UI::readInt());

        if (logic == 1)
            return new AndCriteria(*c1, *c2);
        return new OrCriteria(*c1, *c2);
    }
};
#endif