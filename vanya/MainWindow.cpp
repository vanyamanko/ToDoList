#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QLineEdit>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QLineEdit>
#include <QCalendarWidget>
#include <QTextBrowser>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include "TasksWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCalendarWidget *calendarWidget = ui->calendarWidget;

    QDate minDate(2000, 1, 1);
    QDate maxDate(2100, 12, 31);
    ui->calendarWidget->setMinimumDate(minDate);
    ui->calendarWidget->setMaximumDate(maxDate);

    calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader); //скрыть недели

    int startYear = 2000;
    int endYear = 2100;
    //установка подсказки для пользователя(количество задач) путём подсчёта количества строк в файле(количества задач)
    for (int year = startYear; year <= endYear; ++year) {
        for (int month = 1; month <= 12; ++month) {
            int daysInMonth = QDate(year, month, 1).daysInMonth();

            for (int day = 1; day <= daysInMonth; ++day) {
                QDate date(year, month, day);
                QTextCharFormat cf = calendarWidget->dateTextFormat(date);
                QString tempFile = "/Users/vanyamanko/Desktop/vanya/data/" + date.toString("dd.MM.yyyy") + ".txt";

                int count = countLinesInFile(tempFile);
                QString strCount;
                if(count != 0) strCount = "Amount of tasks: " + QString::number(count);
                else strCount = "You are free!";
                cf.setToolTip(strCount);                       //установка строки
                calendarWidget->setDateTextFormat(date, cf);

            }
        }
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    //создание TaskWindow
    TasksWindow* taskWindow = new TasksWindow(nullptr, date);
    taskWindow->setDate(date);
    taskWindow->setWindowModality(Qt::ApplicationModal);
    taskWindow->show();
    //конкт для обновления подсказки с количеством задач
    connect(taskWindow, &TasksWindow::signalShowAmount, this, &MainWindow::slotShowAmount);
}

void MainWindow::on_pushButtonClose_clicked()
{
    close();
}

void MainWindow::slotShowAmount(int amount, QDate date)
{
    QCalendarWidget *calendarWidget = ui->calendarWidget;

    QTextCharFormat cf = calendarWidget->dateTextFormat(date);
    QString strCount;
    if(amount != 0) strCount = "Amount of tasks: " + QString::number(amount);
    else strCount = "You are free!";
    cf.setToolTip(strCount);                       //установка новой строки в подсказку
    calendarWidget->setDateTextFormat(date, cf);
}
