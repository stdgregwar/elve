#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T14:02:36
#
#-------------------------------------------------

QT       += core gui

TARGET = BlifLoaderPlugin
TEMPLATE = lib
CONFIG += plugin c++11
CONFIG += no_plugin_name_prefix

DESTDIR = ../../Elve/plugins/loaders

SOURCES += BlifLoader.cpp

HEADERS += BlifLoader.h

INCLUDEPATH += ../../Core/interfaces

#DISTFILES += BlifLoaderPlugin.json

unix {
    target.path = /usr/lib
    #target.path = ../Core/plugins
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../Core/ -lElveCore
win32:LIBS += -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt -lboost_filesystem-mt

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core
