#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

class Config
{
private:
    static std::unordered_map<std::string, std::string> values;

public:
    static void load(const std::string &filename);
    static std::string getString(const std::string &key);
    static double getDouble(const std::string &key);
};

#endif
