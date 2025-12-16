#ifndef DATE_H
#define DATE_H

class Date
{
private:
    int day;
    int month;
    int year;

public:
    Date(int d = 1, int m = 1, int y = 2000);

    void print() const;
    int getYear() const;
    int getMonth() const;
    int getDay() const;
};

#endif
