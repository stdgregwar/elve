#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T13:52:40
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ElveCore
TEMPLATE = lib
INCLUDEPATH += $$PWD
DEFINES += ELFE_LIBRARY

CONFIG += c++11
CONFIG += plugin
CONFIG -= debug_and_release

#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -O3

#QMAKE_LFLAGS_RELEASE -= -O1
#QMAKE_LFLAGS_RELEASE += -O3

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:LIBS += -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt -lboost_filesystem-mt


SOURCES +=\
    Constraint.cpp \
    Damp.cpp \
    Graph.cpp \
    Gravity.cpp \
    Movable.cpp \
    Node.cpp \
    PointConstraint.cpp \
    Spring.cpp \
    System.cpp \
    Force.cpp \
    interfaces/LayoutPlugin.cpp \
    Point.cpp \
    QuadTree.cpp \
    QuadTreeNode.cpp \
    PluginManager.cpp \
    GraphWidget.cpp \
    NodeData.cpp \
    GraphData.cpp \
    interfaces/Plugin.cpp \
    interfaces/FileExporterPlugin.cpp \
    Selection.cpp \
    interfaces/LookFactoryPlugin.cpp \
    interfaces/NodeLook.cpp \
    interfaces/EdgeLook.cpp \
    BoxConstraint.cpp \
    interfaces/GraphWidgetListener.cpp \
    LinearConstraint.cpp \
    CommandHistory.cpp \
    ExtendedGraph.cpp \
    NodeDescriptor.cpp

RESOURCES += \
    coreresources.qrc

FORMS +=

DISTFILES += \
    test.blif \
    resources/play.svg \
    resources/pause.svg

HEADERS += \
    Constraint.h \
    Damp.h \
    Graph.h \
    Gravity.h \
    Movable.h \
    Node.h \
    PointConstraint.h \
    Spring.h \
    System.h \
    Force.h \
    interfaces/LayoutPlugin.h \
    Point.h \
    QuadTree.h \
    QuadTreeNode.h \
    PluginManager.h \
    GraphWidget.h \
    Singleton.h \
    utils.h \
    NodeData.h \
    GraphData.h \
    interfaces/Plugin.h \
    interfaces/FileExporterPlugin.h \
    Selection.h \
    interfaces/LookFactoryPlugin.h \
    interfaces/NodeLook.h \
    interfaces/EdgeLook.h \
    BoxConstraint.h \
    interfaces/GraphWidgetListener.h \
    LinearConstraint.h \
    CommandHistory.h \
    ExtendedGraph.h \
    NodeDescriptor.h \
    interfaces/LoaderPlugin.h \
    interfaces/TransformPlugin.h
