#include "GroupManager.h"
#include "AppException.h"
#include "FacultyNumberGenerator.h"
#include "People.h"
#include "Student.h"
#include "PStudent.h"
#include "Logger.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
using namespace std;

void GroupManager::addStudent(int groupNumber, Student *s)
{
    groups[groupNumber].addStudent(s);
}

Group *GroupManager::getGroup(int groupNumber)
{
    auto it = groups.find(groupNumber);
    if (it != groups.end())
        return &it->second;
    return nullptr;
}

const std::map<int, Group> &GroupManager::getAllGroups() const
{
    return groups;
}

Student *GroupManager::findStudentByFacultyNumber(const std::string &fn)
{
    for (auto &pair : groups)
    {
        Student *s = pair.second.findByFacultyNumber(fn);
        if (s)
            return s;
    }
    return nullptr;
}

bool GroupManager::egnExists(const string &egn) const
{
    for (const auto &pair : groups)
    {
        const Group &g = pair.second;
        for (int i = 0; i < g.getCount(); i++)
        {
            if (g.getStudentAt(i)->getEGN() == egn)
                return true;
        }
    }
    return false;
}

void GroupManager::importFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw FileOpenException();

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> parts;

        while (std::getline(ss, token, '|'))
            parts.push_back(token);

        if (parts.size() != 14)
            continue; // невалиден ред → пропускаме

        char type = parts[0][0];
        std::string name = parts[1];
        std::string egn = parts[2];

        if (egnExists(egn))
            continue; // 🔥 ключовото: дублиращо ЕГН → skip

        int d = stoi(parts[3]);
        int m = stoi(parts[4]);
        int y = stoi(parts[5]);

        Gender gender = (parts[6] == "m") ? Gender::Male : Gender::Female;

        double grades[5];
        for (int i = 0; i < 5; i++)
            grades[i] = stod(parts[7 + i]);

        int groupNumber = stoi(parts[12]);
        int enrollYear = stoi(parts[13]);

        std::string fn = FacultyNumberGenerator::generate(groupNumber, enrollYear);
        Date date(d, m, y);

        Student *s = nullptr;
        if (type == 'R')
            s = new Student(name, egn, date, gender, grades, groupNumber, enrollYear, fn);
        else
            s = new PStudent(name, egn, date, gender, grades, groupNumber, enrollYear, fn);

        addStudent(groupNumber, s);
        Logger::log("Импортиран студент: " + name + " FN: " + fn);
    }

    file.close();
}

void GroupManager::exportToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Cannot open file: " << filename << "\n";
        return;
    }

    // Обхождаме всички групи
    for (const auto &groupPair : groups)
    { // groups: std::map<int, Group>
        const Group &group = groupPair.second;

        // Обхождаме всички студенти в групата
        for (int i = 0; i < group.getCount(); ++i)
        {
            const Student *s = group.getStudentAt(i);

            // Тип на студента
            char typeChar = dynamic_cast<const PStudent *>(s) ? 'P' : 'R';
            file << typeChar << "|";

            // Име
            file << s->getName() << "|";

            // Факултетен номер
            file << s->getEGN() << "|";

            // Дата на раждане
            const Date &d = s->getBirthDate();
            file << d.getDay() << "|" << d.getMonth() << "|" << d.getYear() << "|";

            // Пол
            file << (s->getGender() == Gender::Male ? "m" : "f") << "|";

            // 5 оценки
            const double *grades = s->getGrades();
            for (int g = 0; g < 5; ++g)
            {
                file << grades[g];
                if (g < 4)
                    file << "|";
            }

            // Група и година на записване
            file << "|" << s->getGroupNumber() << "|";
            file << s->getEnrollYear() << "\n";
        }
    }

    file.close();
}
