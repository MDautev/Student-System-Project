#include <iostream>
#include "Date.h"

using namespace std;

Date::Date(int d, int m, int y)
{
    day = d;
    month = m;
    year = y;
}

void Date::print() const
{
    cout << day << "/" << month << "/" << year;
}

int Date::getYear() const
{
    return year;
}

int Date::getMonth() const
{
    return month;
}

int Date::getDay() const
{
    return day;
}