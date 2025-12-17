#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger
{
public:
    // Запис на събитие във файл log.txt
    static void log(const std::string &message)
    {
        std::ofstream file("log.txt", std::ios::app); // добавяме в края на файла
        if (file.is_open())
        {
            file << message << std::endl;
            file.close();
        }
    }
};

#endif
