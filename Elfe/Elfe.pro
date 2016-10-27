#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T16:06:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Elfe
TEMPLATE = app


SOURCES += main.cpp \
    MainWindow.cpp \
    PluginManager.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

HEADERS += \
    MainWindow.h \
    PluginManager.h \
    FileLoadAction.h

FORMS += \
    uis/mainwindow_test.ui
