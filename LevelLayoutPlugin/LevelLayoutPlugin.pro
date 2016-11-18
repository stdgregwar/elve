#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T14:23:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = LevelLayoutPlugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../Elve/plugins/layouts

INCLUDEPATH+= ../Core/interfaces
INCLUDEPATH += ../Core

SOURCES += \
    LevelLayout.cpp

HEADERS += \
    LevelLayout.h
DISTFILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
