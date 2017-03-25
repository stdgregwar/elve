#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T14:23:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = LevelLayoutPlugin
TEMPLATE = lib

CONFIG += plugin c++11
CONFIG += no_plugin_name_prefix

DESTDIR = ../../Elve/plugins/layouts

INCLUDEPATH+= ../../Core/interfaces
INCLUDEPATH += ../../Core

LIBS += -L$$OUT_PWD/../../Core/ -lElveCore
win32:LIBS += -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt -lboost_filesystem-mt

SOURCES += \
    LevelLayout.cpp \
    Dialog.cpp

HEADERS += \
    LevelLayout.h \
    Dialog.h
DISTFILES += \
    libLevelLayoutPlugin.ini

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    dialog.ui
