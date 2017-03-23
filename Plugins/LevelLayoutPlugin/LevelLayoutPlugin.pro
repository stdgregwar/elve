#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T14:23:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = LevelLayoutPlugin
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../../Elve/plugins/layouts

INCLUDEPATH+= ../../Core/interfaces
INCLUDEPATH += ../../Core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../../Core/ -lCore

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