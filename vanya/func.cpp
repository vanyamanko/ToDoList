#include "func.h"
void createFile(QFile& file)
{
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
        }
    }
}
void sortTimes(QString times[], int size, int tagArr[], QString date, QListWidget* list) {
    int dop[256];
    for (int i = 0; i<size; i++) {
        QString y = times[i][0];
        y += times[i][1];
        int x = y.toInt();
        y = times[i][3];
        y += times[i][4];
        x *= 60;
        x += (y.toInt());
        dop[i] = x;
    }
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int time1 =  dop[i];
            int time2 = dop[j];
            if (time1 > time2) {
                std::swap(times[j], times[i]);
                std::swap(dop[i], dop[j]);
                std::swap(tagArr[i], tagArr[j]);
                swapLinesInFile("/Users/vanyamanko/Desktop/vanya/tag/" + date + ".txt", i, j);
                swapLinesInFile("/Users/vanyamanko/Desktop/vanya/time/" + date + ".txt", i, j);
                QString way1 = QString::number(i);
                QString way2 = QString::number(j);
                swapFileContents("/Users/vanyamanko/Desktop/vanya/note/" + date + "task" + way1 + ".txt",
                                 "/Users/vanyamanko/Desktop/vanya/note/" + date + "task" + way2 + ".txt");
                swapItems(i, j, list);
            }
        }
    }
}
void sortTags(QString times[], int size, int arr[], QString date, QListWidget* list)
{
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[i] < arr[j]) {
                std::swap(times[j], times[i]);
                std::swap(arr[i], arr[j]);
                swapLinesInFile("/Users/vanyamanko/Desktop/vanya/tag/" + date + ".txt", i, j);
                swapLinesInFile("/Users/vanyamanko/Desktop/vanya/time/" + date + ".txt", i, j);
                QString way1 = QString::number(i);
                QString way2 = QString::number(j);
                swapFileContents("/Users/vanyamanko/Desktop/vanya/note/" + date + "task" + way1 + ".txt",
                                 "/Users/vanyamanko/Desktop/vanya/note/" + date + "task" + way2 + ".txt");
                swapItems(i, j, list);
            }
        }
    }
}
void swapFileContents(const QString &filePath1, const QString &filePath2) {
    QFile file1(filePath1);
    QFile file2(filePath2);
    createFile(file1);
    createFile(file2);
    if (!file1.open(QIODevice::ReadOnly | QIODevice::Text) ||
        !file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Ошибка открытия файлов.";
        return;
    }

    QTextStream stream1(&file1);
    QString contentFile1 = stream1.readAll();

    QTextStream stream2(&file2);
    QString contentFile2 = stream2.readAll();
    file1.close();
    file2.close();
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Ошибка открытия файла для записи.";
        return;
    }
    QTextStream out2(&file2);
    if (!contentFile1.isEmpty()) {
        out2 << contentFile1;
    } else out2 << " ";

    if (!file1.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Ошибка открытия файла для записи.";
        return;
    }
    QTextStream out1(&file1);
    if (!contentFile2.isEmpty()) {
        out1 << contentFile2;
    } else out1 << " ";
    file1.close();
    file2.close();
}
void swapItems(int index1, int index2, QListWidget* listWidget) {

    QListWidgetItem *item1 = listWidget->item(index1);
    QListWidgetItem *item2 = listWidget->item(index2);

    QWidget *widget1 = listWidget->itemWidget(item1);
    QWidget *widget2 = listWidget->itemWidget(item2);

    QLineEdit *lineEdit1 = widget1 ? widget1->findChild<QLineEdit*>() : nullptr;
    QCheckBox *checkBox1 = widget1 ? widget1->findChild<QCheckBox*>() : nullptr;
    QLabel *label1 = widget1 ? widget1->findChild<QLabel*>() : nullptr;

    QLineEdit *lineEdit2 = widget2 ? widget2->findChild<QLineEdit*>() : nullptr;
    QCheckBox *checkBox2 = widget2 ? widget2->findChild<QCheckBox*>() : nullptr;
    QLabel *label2 = widget2 ? widget2->findChild<QLabel*>() : nullptr;

    if (checkBox1 && checkBox2) {
        bool temp = checkBox1->isChecked();
        checkBox1->setChecked(checkBox2->isChecked());
        checkBox2->setChecked(temp);
    }

    if (lineEdit1 && lineEdit2) {
        QString temp = lineEdit1->text();
        lineEdit1->setText(lineEdit2->text());
        lineEdit2->setText(temp);
    }

    if (label1 && label2) {
        QString tempText = label1->text();
        label1->setText(label2->text());
        label2->setText(tempText);

        QString tempColor = label1->styleSheet();
        label1->setStyleSheet(label2->styleSheet());
        label2->setStyleSheet(tempColor);
    }
}

int countLinesInFile(const QString& filename) {
    QFile file(filename);

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&file);
    int lineCount = 0;

    while (!in.atEnd()) {
        in.readLine();
        ++lineCount;
    }

    file.close();
    return lineCount;
}


void deleteLineFromFile(const QString &filePath, int targetIndex)
{

    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "Error opening file for reading and writing.";
        return;
    }

    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        lines << in.readLine();
    }

    if (targetIndex >= lines.size()) {
        return;
    }

    // Удаляем строку из QStringList
    lines.removeAt(targetIndex);

    file.resize(0);

    QTextStream out(&file);
    for (const QString &updatedLine : lines) {
        out << updatedLine << Qt::endl;
    }

    file.close();

}


void deleteDateFile(int index, QString date, int count)
{
    if(count == index)
    {
        QString noteIndex = "/Users/vanyamanko/Desktop/vanya/note/" + date + "task" + QString::number(index) + ".txt";
        QFile fileToDel(noteIndex);
        QTextStream fileToDelStream(&fileToDel);
        if (fileToDel.open(QIODevice::WriteOnly | QIODevice::Text)) {
        fileToDelStream << "";
        }
        fileToDel.close();
    }

    for(int i  = index; i < count; i++)
    {
        QString noteIndex = "/Users/vanyamanko/Desktop/vanya/note/" + date + "task" + QString::number(i) + ".txt";
        QFile fileIndex(noteIndex);
        if (fileIndex.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QString noteNextIndex = "/Users/vanyamanko/Desktop/vanya/note/" + date + "task" + QString::number(i + 1) + ".txt";
            QString bufferNextIndex = readAndClearFile(noteNextIndex);
            QTextStream out(&fileIndex);
            if(bufferNextIndex != ""){
            out << bufferNextIndex;
            } else out << " ";
        }
        fileIndex.close();
    }
}

QString readAndClearFile(QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for reading:" << filePath;
        return QString();
    }

    QTextStream in(&file);
    QString buffer = in.readAll();
    file.close();

    clearFile(filePath);

    return buffer;
}

void clearFile(QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.resize(0);     //установка 0 размера(очиска файла)
        file.close();
    }
}


void swapLinesInFile(const QString& filePath, int lineIndex1, int lineIndex2) {
    // открываем файл для чтения и записи
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    // читаем все строки из файла в список
    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }
    //свап
    QString temp = lines[lineIndex1];
    lines[lineIndex1] = lines[lineIndex2];
    lines[lineIndex2] = temp;

    // перемещаемся в начало файла и перезаписываем его с измененными строками
    file.resize(0);
    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << "\n";
    }
}
