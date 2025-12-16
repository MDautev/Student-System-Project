#ifndef FACULTYNUMBERGENERATOR_H
#define FACULTYNUMBERGENERATOR_H

#include <string>
#include <map>

class FacultyNumberGenerator
{
private:
    static std::map<std::string, int> counters; // ключ = група+година, стойност = последен номер

public:
    // Генерира уникален факултетен номер по група и година
    static std::string generate(int groupNumber, int year);
};

#endif
