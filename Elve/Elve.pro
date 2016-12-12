#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T16:06:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Elve
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    MainWindow.cpp \
    QConsoleWidget.cpp \
    Redirect.cpp \
    uis/Tab.cpp \
    CommandLine.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

#boost
LIBS += -lboost_system -lboost_program_options -lboost_regex -lboost_filesystem

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

HEADERS += \
    MainWindow.h \
    FileLoadAction.h \
    LayoutLoadAction.h \
    QConsoleWidget.h \
    Redirect.h \
    uis/Tab.h \
    CommandLine.h \
    alice/allalice.h

FORMS += \
    uis/mainwindow_test.ui \
    uis/tab.ui

RESOURCES += \
    elveresources.qrc
