#include "TasksWindow.h"
#include "ui_TasksWindow.h"
#include "ui_EditWindow.h"
#include "EditWindow.h"
#include "QTimer"
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QMessageBox>
#include "func.h"

TasksWindow::TasksWindow(QWidget *parent, const QDate& date) :
    QWidget(parent),
    noteDate(date),
    ui(new Ui::TasksWindow)
{
    ui->setupUi(this);

    createDateStr(date); //создание строки даты(00.00.00)

    QString taskFile = "/Users/vanyamanko/Desktop/vanya/data/" + getDateStr() + ".txt";
    QString doneFile = "/Users/vanyamanko/Desktop/vanya/checkBox/" + getDateStr() + ".txt";
    QString tagFile = "/Users/vanyamanko/Desktop/vanya/tag/" + getDateStr() + ".txt";
    QFile file(taskFile);
    QFile fileDone(doneFile);
    QFile fileTag(tagFile);
    bool done;

    createFile(file); //создание файла если его нету
    createFile(fileDone);
    createFile(fileTag);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text) && fileDone.open(QIODevice::ReadOnly | QIODevice::Text)
        && fileTag.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream taskStream(&file);
        QTextStream doneStream(&fileDone);
        QTextStream tagStream(&fileTag);
        while (!taskStream.atEnd()) {
            QString line = taskStream.readLine();
            QString check = doneStream.readLine();
            QString tag = tagStream.readLine(); // считывание строк из файлов
            if(check == '1')
            {
                done = true;
            } else done = false;
            addListItem(line, done, tag.toInt()); // добавление элемента в зависимости от его критериев
        }

        file.close();
        fileDone.close();
    }
    setBaseData();

}


void TasksWindow::setDate(const QDate& date)
{
    QString text = date.toString("dd.MM.yyyy, dddd");
    ui->label->setText(text);
}


TasksWindow::~TasksWindow()
{
    delete ui;
}

void TasksWindow::addListItem(QString text, bool done, int tag)
{
    //создание задачи
    QCheckBox *checkBox = new QCheckBox;
    checkBox->setChecked(done);
    QLineEdit *lineEdit = new QLineEdit(this);
    QLabel* labelTag = new QLabel;

    setTagColor(labelTag, tag);
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(QSize(100, 60));
    QWidget *horizontalWidget = new QWidget;
    // запись в горизонтальный Layout
    QHBoxLayout *horizontalLayout = new QHBoxLayout(horizontalWidget);
    horizontalLayout->addWidget(checkBox);
    horizontalLayout->addWidget(lineEdit);
    horizontalLayout->addWidget(labelTag);

    lineEdit->setText(text);
    // добавление виджета
    ui->listWidget->setItemWidget(item, horizontalWidget);
    //установка размеров
    checkBox->setMinimumHeight(30);
    lineEdit->setMinimumHeight(30);

    lineEdit->setFocus();
    //изменение текста задачи в зависимости от ее выполнености
    if (checkBox->isChecked()) {
        QString textColor = "color: gray;";
        lineEdit->setStyleSheet(textColor);
        countDone++;
    }
    //изменение цвета текста задачи в зависимости от ее выполнености после добавления
    connect(checkBox, &QCheckBox::toggled, [this, lineEdit](bool checked) {
        QString textColor = checked ? "color: gray;" : "";
        lineEdit->setStyleSheet(textColor);
        countDone = checked ? countDone + 1 : countDone - 1;
        QString doneText = "Done: " + QString::number(countDone);
        ui->labelDone->setText(doneText);
    });

    ui->comboBoxSort->setCurrentIndex(0);
    QString amount = QString::number(ui->listWidget->count());
    QString amountText = "Amount of tasks today: " + amount;
    ui->labelAmount->setText(amountText);

    QString doneText = "Done: " + QString::number(countDone);
    ui->labelDone->setText(doneText);
    qDebug() << ui->listWidget->count();
    editParameters.setTag(0, ui->listWidget->count() - 1);
    editParameters.setTime("00.00.00", ui->listWidget->count() - 1);
    editParameters.setNote("", ui->listWidget->count() - 1);
}

void TasksWindow::on_pushButtonAddTask_clicked()
{
    TasksWindow::addListItem("", false, 0);
}


void TasksWindow::closeEvent(QCloseEvent *event) {
    QString taskFile = "/Users/vanyamanko/Desktop/vanya/data/" + getDateStr() + ".txt";
    QFile file(taskFile);
    QString doneFile = "/Users/vanyamanko/Desktop/vanya/checkBox/" + getDateStr() + ".txt";
    QFile fileDone(doneFile);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text) && fileDone.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream streamTask(&file);
        QTextStream streamDone(&fileDone);
        //извлечение из item виджетов и проверка их состояния для записи в файл
        for (int i = 0; i < ui->listWidget->count(); ++i) {

            QListWidgetItem *item = ui->listWidget->item(i);

            QWidget *widget = ui->listWidget->itemWidget(item);

            QLineEdit *lineEdit = widget ? widget->findChild<QLineEdit*>() : nullptr;
            QCheckBox *checkBox = widget ? widget->findChild<QCheckBox*>() : nullptr;

            if(checkBox)
            {
                streamDone << checkBox->isChecked() << Qt::endl;
            }

            if (lineEdit)
            {
                streamTask << lineEdit->text() << Qt::endl;
            }
        }

        file.close();
        fileDone.close();
    }
    emit signalShowAmount(ui->listWidget->count(), noteDate);
    QWidget::closeEvent(event);
}

void TasksWindow::on_listWidget_clicked(const QModelIndex &index)
{
    //узнаём положение мыши
    QPoint contextMenuClickPos = ui->listWidget->viewport()->mapFromGlobal(QCursor::pos());
    //создание контекстного меню и добавление событий(кнопок) в него
    QMenu *contextMenu = new QMenu(this);

    QAction *editAction = new QAction("Edit", this);
    QAction *deleteAction = new QAction("Delete", this);

    contextMenu->addAction(editAction);
    contextMenu->addAction(deleteAction);
    contextMenu->popup(ui->listWidget->viewport()->mapToGlobal(contextMenuClickPos));
    //обработка выбронного действия Edit
    connect(editAction, &QAction::triggered, [=]() {
        createEditWindow(index.row());
        editWindow->setWindowModality(Qt::ApplicationModal);
        editWindow->show();
    });
    //обработка выбронного действия Delete
    connect(deleteAction, &QAction::triggered, [=]() {
        QMessageBox msgBox(this);
        msgBox.setIconPixmap(QPixmap("/Users/vanyamanko/Desktop/vanya/img.png"));
        msgBox.setText("Are you sure you want to delete?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        int reply = msgBox.exec();
        if (reply == QMessageBox::Yes) {
            QString amount = QString::number(ui->listWidget->count() - 1);
            QString amountText = "Amount of tasks today: " + amount;
            ui->labelAmount->setText(amountText);

            QListWidgetItem *item = ui->listWidget->item(index.row());
            QWidget *widget = ui->listWidget->itemWidget(item);
            QCheckBox *checkBox = widget ? widget->findChild<QCheckBox*>() : nullptr;
            if(checkBox->isChecked())
            {
                countDone--;
                QString doneText = "Done: " + QString::number(countDone);
                ui->labelDone->setText(doneText);
            }
            QListWidgetItem *itemDel = ui->listWidget->itemFromIndex(index);
            ui->listWidget->takeItem(ui->listWidget->row(itemDel));
            deleteLineFromFile("/Users/vanyamanko/Desktop/vanya/time/" + getDateStr() + ".txt", index.row());
            deleteLineFromFile("/Users/vanyamanko/Desktop/vanya/tag/" + getDateStr() + ".txt", index.row());
            deleteDateFile(index.row(), getDateStr(), ui->listWidget->count());
        }
    });
}

void TasksWindow::on_comboBoxSort_activated(int index)
{
    editWindow = new EditWindow;
    if(index == 1) //сортировка по времени
    {
        QString time = "/Users/vanyamanko/Desktop/vanya/time/" + getDateStr() + ".txt";
        QFile timeFile(time);
        QString tagFile = "/Users/vanyamanko/Desktop/vanya/tag/" + getDateStr() + ".txt";
        QFile file(tagFile);
        setTimeAndTagFromFile(timeFile, file);
        sortTimes(editParameters.getTimeArr(), ui->listWidget->count(), editParameters.getTagArr(), getDateStr(), ui->listWidget);
        editWindow->setTimeArr(editParameters.getTimeArr());
        editWindow->setTagArr(editParameters.getTagArr());
        editWindow->setNoteArr(editParameters.getNoteArr());
    }

    if(index == 2) //сортировка по тегу
    {
        QString time = "/Users/vanyamanko/Desktop/vanya/time/" + getDateStr() + ".txt";
        QFile timeFile(time);
        QString tagFile = "/Users/vanyamanko/Desktop/vanya/tag/" + getDateStr() + ".txt";
        QFile file(tagFile);
        setTimeAndTagFromFile(timeFile, file);
        sortTags(editParameters.getTimeArr(), ui->listWidget->count(), editParameters.getTagArr(), getDateStr(), ui->listWidget);
        editWindow->setTimeArr(editParameters.getTimeArr());
        editWindow->setTagArr(editParameters.getTagArr());
        editWindow->setNoteArr(editParameters.getNoteArr());
    }
    for(int i=0; i < ui->listWidget->count(); i++)
    {
    createEditWindow(i);
    }
    editWindow->on_pushButtonSave_clicked();
}


void TasksWindow::on_saveButton_clicked()
{
    close();
}

void TasksWindow::slotLine(QString line, int i) //слот для передачи информации о тексте задачи в EdirWindow
{
    QListWidgetItem *item = ui->listWidget->item(i);

    if (item) {
        QWidget *widget = ui->listWidget->itemWidget(item);

        if (widget) {
            QLineEdit *lineEdit = widget->findChild<QLineEdit*>();

            if (lineEdit) {
                lineEdit->setText(line);
            }
        }
    }
}

void TasksWindow::slotTag(int tagIndex, int i) //слот для передачи информации о теге задачи в EdirWindow
{
    QListWidgetItem *item = ui->listWidget->item(i);

    if (item) {
        QWidget *widget = ui->listWidget->itemWidget(item);

        if (widget) {
            QLabel *label = widget->findChild<QLabel*>();

            if (label) {
            setTagColor(label, tagIndex);
            }
        }
    }
}

void TasksWindow::createDateStr(QDate date)
{
    int day = date.day();
    int month = date.month();
    int year = date.year();

    QString text = QString("%1.%2.%3")
    .arg(day, 2, 10, QChar('0'))
    .arg(month, 2, 10, QChar('0'))
    .arg(year, 4, 10, QChar('0'));
    setDateStr(text);
}

void TasksWindow::setBaseData()
{
    for(int i=0; i<256; i++)
    {
        editParameters.setTag(0, i);
    }
    for(int i=0; i<256; i++)
    {
        editParameters.setTime("00.00.00", i);
    }
    for(int i=0; i<256; i++)
    {
        editParameters.setNote("", i);
    }
}


void TasksWindow::setTagColor(QLabel* labelTag, int index)
{
        labelTag->setText(" ●");
        if(index == 0)
        {
            labelTag->setText("");
        }
        if(index == 1)
        {
            labelTag->setStyleSheet("color: lime;"); //green
        }
        if(index == 2)
        {
            labelTag->setStyleSheet("color: rgb(252, 195, 3);"); //yellow
        }
        if(index == 3)
        {
            labelTag->setStyleSheet("color: red;");
        }
}

void TasksWindow::setTimeAndTagFromFile(QFile& timeFile, QFile& tagFile)
{
    createFile(timeFile);
    createFile(tagFile);

    if (tagFile.open(QIODevice::ReadOnly | QIODevice::Text) && timeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QTextStream timeStream(&timeFile);
        QTextStream tagStream(&tagFile);
        int i=0;
        while (!tagStream.atEnd()) {

        QString tagIndex = tagStream.readLine();
        QString timeLine = timeStream.readLine();

        editParameters.setTag(tagIndex.toInt(), i);
        editParameters.setTime(timeLine, i);
        i++;
        }

        tagFile.close();
        timeFile.close();
    }
}

void TasksWindow::createEditWindow(int index)
{
    editWindow = new EditWindow;
    editWindow->setDateStr(getDateStr());
    editWindow->setCountItem(ui->listWidget->count());

    QString tagFile = "/Users/vanyamanko/Desktop/vanya/tag/" + getDateStr() + ".txt";
    QFile file(tagFile);

    QString time = "/Users/vanyamanko/Desktop/vanya/time/" + getDateStr() + ".txt";
    QFile timeFile(time);
    QString way = QString::number(index);
    QString note = "/Users/vanyamanko/Desktop/vanya/note/" + getDateStr() + "task" + way + ".txt";
    QFile noteFile(note);

    setTimeAndTagFromFile(timeFile, file);
    //установка конекта между сигналом EditWindow и слотом TaskWindow информации о теге и тексте задачи в EditWindow
    connect(editWindow, &EditWindow::signalLine, this, &TasksWindow::slotLine);
    connect(editWindow, &EditWindow::signalTag, this, &TasksWindow::slotTag);

    createFile(noteFile);
    if (noteFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&noteFile);

        while (!in.atEnd()) {
        QString noteLine = in.readAll();
        editParameters.setNote(noteLine, index);
        }
        noteFile.close();
    }

    //установка параметром в EditWindow
    editWindow->setClickIndex(index);
    editWindow->setNoteArr(editParameters.getNoteArr());
    editWindow->setTagArr(editParameters.getTagArr());
    editWindow->setTimeArr(editParameters.getTimeArr());
    editWindow->setInterface(index);
    ui->comboBoxSort->setCurrentIndex(0);


    //установка текста задачи в окно EditWindow
    QListWidgetItem *item = ui->listWidget->item(index);

    QWidget *widget = ui->listWidget->itemWidget(item);

    QLineEdit *lineEdit = widget ? widget->findChild<QLineEdit*>() : nullptr;

    editWindow->ui->lineEdit->setText(lineEdit->text());
}
