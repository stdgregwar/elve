#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T19:25:49
#
#-------------------------------------------------

QT       += core gui widgets svg xml

TARGET = BasicLookPlugin
TEMPLATE = lib
CONFIG += plugin c++11
CONFIG += no_plugin_name_prefix

DESTDIR = ../../Elve/plugins/looks

#DISTFILES += BlifLoaderPlugin.json

unix {
    target.path = ~/.elve/plugins/looks
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../Core/ -lElveCore
win32:LIBS += -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt -lboost_filesystem-mt

INCLUDEPATH += $$PWD/../../Core
DEPENDPATH += $$PWD/../../Core

HEADERS += \
    BasicLookPlugin.h \
    BasicEdgeLook.h \
    BasicNodeLook.h

SOURCES += \
    BasicLookPlugin.cpp \
    BasicEdgeLook.cpp \
    BasicNodeLook.cpp

RESOURCES += \
    basiclookres.qrc
