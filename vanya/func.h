#ifndef FUNC_H
#define FUNC_H
#include "QString"
#include "QIODevice"
#include "QDebug"
#include <QTime>
#include <QListWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QStringList>
void createFile(QFile& file);
void sortTimes(QString times[], int size, int tagArr[], QString date, QListWidget* list);
void swapFileContents(const QString &filePath1, const QString &filePath2);
void swapItems(int index1, int index2, QListWidget* listWidget);
void sortTags(QString times[], int size, int arr[], QString date, QListWidget* list);
int countLinesInFile(const QString& filename);
void deleteLineFromFile(const QString &filePath, int targetIndex);
void deleteDateFile(int index, QString date, int count);
QString readAndClearFile(QString &filePath);
void clearFile(QString &filePath);
void swapLinesInFile(const QString& filePath, int lineIndex1, int lineIndex2);
#endif
