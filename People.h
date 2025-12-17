#ifndef PEOPLE_H
#define PEOPLE_H

#include <string>
#include "Date.h"

enum class Gender
{
    Male,
    Female
};

class People
{
protected:
    std::string name;
    std::string EGN;
    Date birthDate;
    Gender gender;

public:
    People(const std::string &n, const std::string &egn, const Date &d, Gender g);

    virtual void print() const;
    int getAge() const;

public:
    static bool validateEGN(const std::string &egn,
                            Date &outDate,
                            Gender &outGender);

    virtual ~People();
    std::string getName() const;
    Gender getGender() const;
};

#endif
