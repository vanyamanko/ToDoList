#include "EditWindow.h"
#include "TasksWindow.h"
#include "ui_TasksWindow.h"
#include "ui_EditWindow.h"
#include "MainWindow.h"
#include <algorithm>
#include <string.h>
EditWindow::EditWindow(QWidget *parent) : QWidget(parent), ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    setComboBoxColor();
}


EditWindow::~EditWindow()
{
    delete ui;
    delete parent();
}

void EditWindow::setInterface(int i) {

    int tag = getTag(i);
    ui->comboBox->setCurrentIndex(tag);

    QTime time = QTime::fromString(getTime(i), "hh:mm:ss");
    ui->timeEdit->setTime(time);

    ui->textEdit->setText(getNote(i));
}

void EditWindow::setCountItem(int count)
{
    countItem = count;
}

void EditWindow::setClickIndex(int index)
{
    clickIndex = index;
}

int EditWindow::getCountItem()
{
    return countItem;
}

int EditWindow::getClickIndex()
{
    return clickIndex;
}
void EditWindow::on_comboBox_activated(int index)
{
    setTag(index, clickIndex);
}

void EditWindow::on_timeEdit_userTimeChanged(const QTime &time)
{
   setTime(time.toString(), clickIndex);
}

void EditWindow::on_textEdit_textChanged()
{
    QString newText = ui->textEdit->toPlainText();
    if(newText == "")
    {
        newText = " ";
    }
    setNote(newText, clickIndex);
}

void EditWindow::on_pushButtonSave_clicked()
{
    pushDataInFile();
    emit signalLine(ui->lineEdit->text(), clickIndex); //сигнал для передачи нового текста задачи в окно TaskWindow
    emit signalTag(ui->comboBox->currentIndex(), clickIndex); //сигнал для передачи нового тега задачи в окно TaskWindow
    close();
}

void EditWindow::on_pushButtonDoNotSave_clicked()
{
      close();
}

void EditWindow::setComboBoxColor()
{
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        QPalette palette = ui->comboBox->palette();
        switch (index) {
        case 1:
            palette.setColor(QPalette::Text, Qt::green);
            break;
        case 2:
            palette.setColor(QPalette::Text, QColor(252, 195, 3)); //yellow
            break;
        case 3:
            palette.setColor(QPalette::Text, Qt::red);
            break;
        default:
            palette.setColor(QPalette::Text, Qt::black);
            break;
        }
        ui->comboBox->setPalette(palette);
    });
}
void EditWindow::pushDataInFile()
{
    QString tag = "/Users/vanyamanko/Desktop/vanya/tag/" + getDateStr() + ".txt";
    QFile fileTag(tag);

    QString time = "/Users/vanyamanko/Desktop/vanya/time/" + getDateStr() + ".txt";
    QFile fileTime(time);

    QString way = QString::number(clickIndex);
    QString note = "/Users/vanyamanko/Desktop/vanya/note/" + getDateStr() + "task" + way + ".txt";

    QFile fileNote(note);

    if (fileTag.open(QIODevice::WriteOnly | QIODevice::Text) && fileTime.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream tagStream(&fileTag);
        QTextStream timeStream(&fileTime);
        for (int i = 0; i < countItem; ++i) {
            tagStream << getTag(i) << Qt::endl;
            timeStream << getTime(i) << Qt::endl;
        }
        fileTag.close();
        fileTime.close();
    }

    if (fileNote.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream noteStream(&fileNote);
        noteStream << getNote(clickIndex);
        fileNote.close();
    }
}



