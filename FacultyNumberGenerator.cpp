#include "FacultyNumberGenerator.h"
#include <sstream>
#include <iomanip>

std::map<std::string, int> FacultyNumberGenerator::counters;

std::string FacultyNumberGenerator::generate(int groupNumber, int year)
{
    // ключ по група и година
    std::ostringstream keyStream;
    keyStream << std::setw(2) << std::setfill('0') << groupNumber
              << "00"
              << std::setw(2) << std::setfill('0') << (year % 100);
    std::string key = keyStream.str();

    // последователен номер
    int number = 1;
    if (counters.find(key) != counters.end())
        number = counters[key] + 1;

    counters[key] = number;

    // окончателен факултетен номер
    std::ostringstream fn;
    fn << key << std::setw(3) << std::setfill('0') << number;
    return fn.str();
}
