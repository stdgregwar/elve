#ifndef LAYOUTPLUGIN_H
#define LAYOUTPLUGIN_H

#include <unordered_map>
#include <Graph.h>
#include <QVector2D>
#include <QtPlugin>

#include <EdgeItem.h>
#include <NodeItem.h>

typedef std::unordered_map<NodeID,QVector2D> NodePositions;

#define LayoutPlugin_iid "ch.epfl.lap.elfe.LayoutPlugin"

class LayoutPlugin
{
public:
    virtual void setGraph(SharedGraph graph);
    virtual void setGraph(SharedGraph g,const NodePositions& positions) = 0;
    virtual void quickSim(size_t ticks);
    virtual void tick(float dt, bool fast) = 0;
    virtual QString layoutName() = 0;
private:
};

Q_DECLARE_INTERFACE(LayoutPlugin,LayoutPlugin_iid)

#endif // LAYOUTPLUGIN_H
