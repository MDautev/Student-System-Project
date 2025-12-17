#include <iostream>
#include "People.h"
#include <ctime>

using namespace std;

People::People(const string &n, const string &egn, const Date &d, Gender g)
    : name(n), EGN(egn), birthDate(d), gender(g) {}

void People::print() const
{
    cout << "Име: " << name
         << ", ЕГН: " << EGN
         << ", Пол: " << (gender == Gender::Male ? "Мъж" : "Жена")
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

Gender People::getGender() const
{
    return gender;
}

bool People::validateEGN(const std::string &egn, Date &outDate, Gender &outGender)
{
    if (egn.length() != 10)
        return false;

    for (char c : egn)
        if (!isdigit(c))
            return false;

    int yy = stoi(egn.substr(0, 2));
    int mm = stoi(egn.substr(2, 2));
    int dd = stoi(egn.substr(4, 2));

    int year;
    if (mm >= 1 && mm <= 12)
        year = 1900 + yy;
    else if (mm >= 21 && mm <= 32)
    {
        year = 1800 + yy;
        mm -= 20;
    }
    else if (mm >= 41 && mm <= 52)
    {
        year = 2000 + yy;
        mm -= 40;
    }
    else
        return false;

    if (!Date::isValid(dd, mm, year))
        return false;

    // Пол
    int genderDigit = egn[8] - '0';
    outGender = (genderDigit % 2 == 0) ? Gender::Male : Gender::Female;

    // Контролна цифра
    int weights[9] = {2, 4, 8, 5, 10, 9, 7, 3, 6};
    int sum = 0;
    for (int i = 0; i < 9; i++)
        sum += (egn[i] - '0') * weights[i];

    int control = sum % 11;
    if (control == 10)
        control = 0;

    return control == (egn[9] - '0');
}

People::~People() {}
