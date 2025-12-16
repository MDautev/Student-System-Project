#include <iostream>
#include "People.h"
#include <ctime>

using namespace std;

People::People(const string &n, const string &egn, const Date &d)
    : name(n), EGN(egn), birthDate(d) {}

void People::print() const
{
    cout << "Име: " << name
         << ", ЕГН: " << EGN
         << ", Рождена дата: ";
    birthDate.print();
    cout << endl;
}

int People::getAge() const
{
    // взимаме текущата дата
    time_t t = time(nullptr);
    tm *now = localtime(&t);

    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    int age = currentYear - birthDate.getYear();

    // проверка дали рожденият ден е минал
    if (currentMonth < birthDate.getMonth() ||
        (currentMonth == birthDate.getMonth() && currentDay < birthDate.getDay()))
    {
        age--;
    }

    return age;
}

string People::getName() const
{
    return name;
}

People::~People() {}
