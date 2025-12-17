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

bool Date::isValid(int d, int m, int y)
{
    if (y < 1800 || y > 2099)
        return false;
    if (m < 1 || m > 12)
        return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // проверка за високосна година
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
        daysInMonth[1] = 29;

    return d >= 1 && d <= daysInMonth[m - 1];
}
