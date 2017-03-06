#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T19:25:49
#
#-------------------------------------------------

QT       += core gui widgets svg

TARGET = BlockLookPlugin
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../Elve/plugins/looks

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
    BlockNodeLook.h \
    BlockEdgeLook.h \
    BlockLookPlugin.h

SOURCES += \
    BlockNodeLook.cpp \
    BlockEdgeLook.cpp \
    BlockLookPlugin.cpp
