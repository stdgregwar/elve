#ifndef LAYOUTPLUGIN_H
#define LAYOUTPLUGIN_H

#include <unordered_map>
#include <Graph.h>
#include <QVector2D>
#include <QtPlugin>

#include <EdgeItem.h>
#include <NodeItem.h>
#include <System.h>

#include "LayoutPluginFactory.h"

//#define LayoutPlugin_iid "ch.epfl.lap.elfe.LayoutPlugin"

class LayoutPlugin
{
public:
    void clear();
    virtual void setGraph(SharedGraph graph) = 0;
    QVector2D startPosition(const NodeID& id, QRectF rect = QRectF(0,0,1024,1024));
    void setGraph(SharedGraph g,const NodePositions& positions);
    virtual void quickSim(size_t ticks);
    virtual void tick(float dt, bool fast);
    virtual QString layoutName() = 0;
    System& system();
    const System& system() const;
    virtual ~LayoutPlugin(){}
private:
    System mSystem;
    NodePositions mStartPositions;
};

//Q_DECLARE_INTERFACE(LayoutPlugin,LayoutPlugin_iid)

#endif // LAYOUTPLUGIN_H
