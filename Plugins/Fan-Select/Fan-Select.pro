#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T19:25:49
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = FanSelect
TEMPLATE = lib
CONFIG += plugin c++11
CONFIG += no_plugin_name_prefix

DESTDIR = ../../Elve/plugins/transforms

unix {
    target.path = ~/.elve/plugins/transforms
    INSTALLS += target
}

INCLUDEPATH += ../../Core

#DISTFILES += BlifLoaderPlugin.json

LIBS += -L$$OUT_PWD/../../Core/ -lElveCore
win32:LIBS += -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt -lboost_filesystem-mt

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

HEADERS += \
    FanSelect.h

SOURCES += \
    FanSelect.cpp
