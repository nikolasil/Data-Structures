#ifndef DATE_H
#define DATE_H

#include <string>

using namespace std;

class date
{
public:
    date(string dd, string mm, string yy);
    date(string d);
    ~date();

    void print();
    int compare(date d);

private:
    string day;
    string month;
    string year;
};

#endif