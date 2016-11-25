#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T14:23:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = SimpleLayoutPlugin
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../Elve/plugins/layouts

INCLUDEPATH+= ../Core/interfaces
INCLUDEPATH += ../Core

SOURCES += \
    SimpleLayout.cpp

HEADERS += \
    SimpleLayout.h
DISTFILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
