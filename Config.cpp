#include "Config.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::unordered_map<std::string, std::string> Config::values;

void Config::load(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Не може да се отвори config файлът");

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        values[key] = value;
    }
}

std::string Config::getString(const std::string &key)
{
    return values.at(key);
}

double Config::getDouble(const std::string &key)
{
    return std::stod(values.at(key));
}
