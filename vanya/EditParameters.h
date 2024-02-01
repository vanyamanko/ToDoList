#ifndef EDITPARAMETERS_H
#define EDITPARAMETERS_H
#include <QString>
#include <QWidget>
#include "Date.h"
#include "List.h"
class EditParameters : public Date
{
    List<int> tagArr;
    List<QString> timeArr;
    List<QString> noteArr;
public:
    EditParameters(): tagArr(256), timeArr(256),noteArr(256){}
    int getTag(int index);
    int* getTagArr();
    void setTag(int tag, int index);
    void setTagArr(int tag[]);

    QString getTime(int index);
    QString* getTimeArr();
    void setTime(QString time, int index);
    void setTimeArr(QString time[]);

    QString getNote(int index);
    QString* getNoteArr();
    void setNote(QString note, int index);
    void setNoteArr(QString note[]);
};

#endif
