#include "Logger.h"
#include "Config.h"
#include <fstream>
#include <iostream>

using namespace std;

void Logger::log(const std::string &message)
{
    std::ofstream file(Config::getString("log_file"), std::ios::app);
    if (file.is_open())
    {
        file << message << std::endl;
    }
}

void Logger::readLogs()
{
    std::ifstream file(Config::getString("log_file"));

    if (!file.is_open())
    {
        std::cout << "Няма лог файл или не може да бъде отворен.\n";
        return;
    }

    std::cout << "\n===== LOG FILE =====\n";
    std::string line;
    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
    }
    std::cout << "====================\n";
}