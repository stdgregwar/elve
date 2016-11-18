#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T16:06:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Elve
TEMPLATE = app


SOURCES += main.cpp \
    MainWindow.cpp \
    PluginManager.cpp \
    GraphWidget.cpp \
    QConsoleWidget.cpp \
    Redirect.cpp \
    uis/Tab.cpp \
    CommandLine.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

#boost
LIBS += -lboost_system -lboost_program_options

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

HEADERS += \
    MainWindow.h \
    PluginManager.h \
    FileLoadAction.h \
    LayoutLoadAction.h \
    alice/commands/alias.hpp \
    alice/commands/convert.hpp \
    alice/commands/current.hpp \
    alice/commands/help.hpp \
    alice/commands/print.hpp \
    alice/commands/ps.hpp \
    alice/commands/quit.hpp \
    alice/commands/read_io.hpp \
    alice/commands/set.hpp \
    alice/commands/show.hpp \
    alice/commands/store.hpp \
    alice/commands/write_io.hpp \
    alice/alice.hpp \
    alice/command.hpp \
    alice/readline.hpp \
    alice/rules.hpp \
    GraphWidget.h \
    QConsoleWidget.h \
    Redirect.h \
    uis/Tab.h \
    CommandLine.h

FORMS += \
    uis/mainwindow_test.ui \
    uis/tab.ui

RESOURCES += \
    elveresources.qrc
