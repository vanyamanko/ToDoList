#ifndef TASKSWINDOW_H
#define TASKSWINDOW_H

#include <QWidget>
#include <QDate>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QScrollArea>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include "ui_TasksWindow.h"
#include "ui_EditWindow.h"
#include "QMainWindow"
#include "func.h"
#include "EditWindow.h"
#include "EditParameters.h"
namespace Ui {
class TasksWindow;
}

class TasksWindow : public QWidget, public Date
{
    Q_OBJECT

    void addListItem(QString text, bool done, int tag);
    QDate noteDate;

    EditWindow* editWindow = nullptr;
public:
    Ui::TasksWindow *ui;
    explicit TasksWindow(QWidget *parent = nullptr, const QDate& date = QDate());
    void setDate(const QDate&);
    ~TasksWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_pushButtonAddTask_clicked();
    void on_listWidget_clicked(const QModelIndex &index);

    void on_comboBoxSort_activated(int index);

    void on_saveButton_clicked();

private:
    int countDone = 0;
    void createDateStr(QDate date);
    void setBaseData();
    void setTagColor(QLabel* labelTag, int index);
    void setTimeAndTagFromFile(QFile& timeFile, QFile& tagFile);
    void createEditWindow(int index);
public slots:
    void slotLine(QString line, int i);
    void slotTag(int tagIndex, int i);
signals:
    void signalShowAmount(int amount, QDate date);
public:
    EditParameters editParameters;
};

#endif
