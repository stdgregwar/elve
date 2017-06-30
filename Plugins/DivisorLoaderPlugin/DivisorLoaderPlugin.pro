#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T14:02:36
#
#-------------------------------------------------

QT       += core gui

TARGET = DivisorLoaderPlugin
TEMPLATE = lib
CONFIG += plugin c++11
CONFIG += no_plugin_name_prefix

DESTDIR = ../../Elve/plugins/loaders

unix {
    target.path = ~/.elve/plugins/loaders
    INSTALLS += target
}

SOURCES += \
    DivisorLoader.cpp

HEADERS += \
    DivisorLoader.h

INCLUDEPATH += ../../Core/interfaces

#DISTFILES += BlifLoaderPlugin.json

LIBS += -L$$OUT_PWD/../../Core/ -lElveCore
win32:LIBS += -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt -lboost_filesystem-mt

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core
