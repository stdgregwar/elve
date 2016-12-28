#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T19:25:49
#
#-------------------------------------------------

QT       += core gui widgets svg

TARGET = SvgExporterPlugin
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../Elve/plugins/exporters

INCLUDEPATH += ../Core/interfaces

#DISTFILES += BlifLoaderPlugin.json

unix {
    target.path = /usr/lib
    #target.path = ../Core/plugins
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

HEADERS += \
    SvgExporterPlugin.h

SOURCES += \
    SvgExporterPlugin.cpp
