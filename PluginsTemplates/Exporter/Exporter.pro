#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T19:25:49
#
#-------------------------------------------------

QT       += core gui widgets svg

TARGET = ExporterName #your plugin name here
TEMPLATE = lib

CONFIG += plugin c++11
CONFIG += no_plugin_name_prefix

# DESTDIR = ../../Elve/plugins/exporters #uncomment this when writting your plugin

INCLUDEPATH += ../../Core

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../Core/ -lElveCore
win32:LIBS += -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt -lboost_filesystem-mt

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

HEADERS += \
    Exporter.h

SOURCES += \
    Exporter.cpp
