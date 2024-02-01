QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    EditParameters.cpp \
    Exception.cpp \
    List.cpp \
    TasksWindow.cpp \
    Date.cpp \
    EditWindow.cpp \
    func.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Exception.h \
    List.h \
    TasksWindow.h \
    Date.h \
    EditWindow.h \
    EditParameters.h \
    func.h \
    MainWindow.h

FORMS += \
    TasksWindow.ui \
    EditWindow.ui \
    MainWindow.ui

macx: ICON = /Users/vanyamanko/Desktop/vanya/im.ico


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
