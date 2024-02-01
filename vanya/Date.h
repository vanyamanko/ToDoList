#ifndef DATE_H
#define DATE_H
#include <QString>

class Date
{
    QString dateStr;
public:
    QString getDateStr();
    void setDateStr(QString str);
};

#endif
