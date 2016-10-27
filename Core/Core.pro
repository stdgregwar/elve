#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T13:52:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Core
TEMPLATE = lib
INCLUDEPATH += $$PWD
DEFINES += ELFE_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}


SOURCES +=\
    Constraint.cpp \
    Damp.cpp \
    EdgeItem.cpp \
    Graph.cpp \
    GraphWidget.cpp \
    Gravity.cpp \
    LayoutPolicy.cpp \
    LevelForceLayout.cpp \
    Mass.cpp \
    Movable.cpp \
    Node.cpp \
    NodeItem.cpp \
    PointConstraint.cpp \
    SimpleForceLayout.cpp \
    Spring.cpp \
    System.cpp \
    VerticalConstraint.cpp \
    Force.cpp

RESOURCES += \
    resources.qrc

FORMS +=

DISTFILES += \
    test.blif

HEADERS += \
    Constraint.h \
    Damp.h \
    EdgeItem.h \
    Graph.h \
    GraphWidget.h \
    Gravity.h \
    LayoutPolicy.h \
    LevelForceLayout.h \
    Mass.h \
    Movable.h \
    Node.h \
    NodeItem.h \
    PointConstraint.h \
    SimpleForceLayout.h \
    Spring.h \
    System.h \
    VerticalConstraint.h \
    Force.h \
    interfaces/GraphLoader.h
