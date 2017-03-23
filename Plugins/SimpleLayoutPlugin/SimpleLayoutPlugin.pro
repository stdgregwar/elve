#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T14:23:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = SimpleLayoutPlugin
TEMPLATE = lib
CONFIG += plugin c++11
CONFIG += no_plugin_name_prefix

DESTDIR = ../../Elve/plugins/layouts

INCLUDEPATH+= ../../Core/interfaces
INCLUDEPATH += ../../Core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Core/release/ -lElveCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Core/debug/ -lElveCore
else:unix: LIBS += -L$$OUT_PWD/../../Core/ -lElveCore

SOURCES += \
    SimpleLayout.cpp

HEADERS += \
    SimpleLayout.h
DISTFILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
