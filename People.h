#ifndef PEOPLE_H
#define PEOPLE_H

#include <string>
#include "Date.h"

class People
{
protected:
    std::string name;
    std::string EGN;
    Date birthDate;

public:
    People(const std::string &n, const std::string &egn, const Date &d);

    virtual void print() const;
    int getAge() const;
    virtual ~People();
    std::string getName() const;
};

#endif
