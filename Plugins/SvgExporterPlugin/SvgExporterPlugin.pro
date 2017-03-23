#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T19:25:49
#
#-------------------------------------------------

QT       += core gui widgets svg

TARGET = SvgExporterPlugin
TEMPLATE = lib

CONFIG += plugin c++11
CONFIG += no_plugin_name_prefix

DESTDIR = ../../Elve/plugins/exporters

INCLUDEPATH += ../../Core

#DISTFILES += BlifLoaderPlugin.json

unix {
    target.path = /usr/lib
    #target.path = ../Core/plugins
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Core/release/ -lElveCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Core/debug/ -lElveCore
else:unix: LIBS += -L$$OUT_PWD/../../Core/ -lElveCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

HEADERS += \
    SvgExporterPlugin.h

SOURCES += \
    SvgExporterPlugin.cpp
