#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T14:23:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = LevelLayoutPlugin
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../Elve/plugins/layouts

INCLUDEPATH+= ../Core/interfaces
INCLUDEPATH += ../Core

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
