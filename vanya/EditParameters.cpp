#include "EditParameters.h"
#include <QString>


int EditParameters::getTag(int index)
{
    return tagArr[index];
}

int* EditParameters::getTagArr()
{
    return tagArr.data();
}

void EditParameters::setTag(int tag, int index)
{
    tagArr[index] = tag;
}

void EditParameters::setTagArr(int tag[])
{
    for(int i=0; i < 256; i++)
    {
        tagArr[i] = tag[i];
    }
}

QString EditParameters::getTime(int index)
{
    return timeArr[index];
}

QString* EditParameters::getTimeArr()
{
    return timeArr.data();
}

void EditParameters::setTime(QString time, int index)
{
    timeArr[index] = time;
}

void EditParameters::setTimeArr(QString time[])
{
    for(int i=0; i < 256; i++)
    {
        timeArr[i] = time[i];
    }
}

QString EditParameters::getNote(int index)
{
    return noteArr[index];
}

QString* EditParameters::getNoteArr()
{
    return noteArr.data();
}

void EditParameters::setNote(QString note, int index)
{
    noteArr[index] = note;
}


void EditParameters::setNoteArr(QString note[])
{
    for(int i=0; i < 256; i++)
    {
        noteArr[i] = note[i];
    }
}
