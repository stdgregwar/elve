#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T16:06:16
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Elve
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    MainWindow.cpp \
    QConsoleWidget.cpp \
    Redirect.cpp \
    CommandLine.cpp \
    StoreView.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lElveCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lElveCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lElveCore

#boost
unix:LIBS += -lboost_system -lboost_program_options -lboost_regex -lboost_filesystem
win32:LIBS += -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt -lboost_filesystem-mt

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

HEADERS += \
    MainWindow.h \
    FileLoadAction.h \
    LayoutLoadAction.h \
    QConsoleWidget.h \
    Redirect.h \
    CommandLine.h \
    alice/allalice.h \
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
    StoreView.h \
    FileExportAction.h \
    TransformAction.h \
    LookLoadAction.h \
    commands/Cluster.h \
    commands/Export.h \
    commands/Group.h \
    commands/Layout.h \
    commands/Loader.h \
    commands/Plugin.h \
    commands/Select.h \
    commands/Transform.h \
    commands/Ungroup.h \
    commands/Chrono.h \
    commands/ClearAll.h \
    commands/Look.h

FORMS += \
    uis/mainwindow_test.ui

RESOURCES += \
    elveresources.qrc
