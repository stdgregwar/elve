#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T13:52:40
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Core
TEMPLATE = lib
INCLUDEPATH += $$PWD
DEFINES += ELFE_LIBRARY

CONFIG += c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}


SOURCES +=\
    Constraint.cpp \
    Damp.cpp \
    EdgeItem.cpp \
    Graph.cpp \
    Gravity.cpp \
    Movable.cpp \
    Node.cpp \
    NodeItem.cpp \
    PointConstraint.cpp \
    Spring.cpp \
    System.cpp \
    VerticalConstraint.cpp \
    Force.cpp \
    interfaces/LayoutPlugin.cpp \
    Point.cpp \
    QuadTree.cpp \
    QuadTreeNode.cpp \
    EGraph.cpp \
    PluginManager.cpp \
    GraphWidget.cpp \
    NodeData.cpp \
    GraphData.cpp \
    interfaces/Plugin.cpp \
    interfaces/FileExporterPlugin.cpp \
    Selection.cpp

RESOURCES += \
    coreresources.qrc

FORMS +=

DISTFILES += \
    test.blif

HEADERS += \
    Constraint.h \
    Damp.h \
    EdgeItem.h \
    Graph.h \
    Gravity.h \
    Movable.h \
    Node.h \
    NodeItem.h \
    PointConstraint.h \
    Spring.h \
    System.h \
    VerticalConstraint.h \
    Force.h \
    interfaces/LayoutPlugin.h \
    interfaces/GraphLoaderPlugin.h \
    Point.h \
    QuadTree.h \
    QuadTreeNode.h \
    EGraph.h \
    PluginManager.h \
    GraphWidget.h \
    Singleton.h \
    utils.h \
    NodeData.h \
    GraphData.h \
    interfaces/Plugin.h \
    interfaces/FileExporterPlugin.h \
    Selection.h \
    interfaces/GraphTransformPlugin.h
