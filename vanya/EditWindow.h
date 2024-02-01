#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>
#include "EditParameters.h"
namespace Ui {
class EditWindow;
}

class EditWindow : public QWidget, public EditParameters
{
    Q_OBJECT
public:
    Ui::EditWindow *ui;


    explicit EditWindow(QWidget *parent = nullptr);
    ~EditWindow();
    void setInterface(int index);
    void setCountItem(int count);
    void setClickIndex(int index);
    int getClickIndex();
    int getCountItem();

private slots:
    void on_comboBox_activated(int index);
    void on_timeEdit_userTimeChanged(const QTime &time);

    void on_textEdit_textChanged();

    void on_pushButtonDoNotSave_clicked();

private:
    int clickIndex;
    int countItem;
    void setComboBoxColor();
    void pushDataInFile();
signals:
    void signalLine(QString line, int i);
    void signalTag(int tagIndex, int i);
public slots:
    void on_pushButtonSave_clicked();
};
#endif
