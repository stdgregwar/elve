#ifndef LAYOUTPLUGIN_H
#define LAYOUTPLUGIN_H

#include <unordered_map>
#include <Graph.h>
#include <QVector2D>
#include <QtPlugin>

#include <EdgeItem.h>
#include <NodeItem.h>
#include <System.h>



#define LayoutPlugin_iid "ch.epfl.lap.elfe.LayoutPlugin"

class LayoutPlugin
{
public:
    void clear();
    virtual void setGraph(SharedGraph graph);
    virtual void setGraph(SharedGraph g,const NodePositions& positions) = 0;
    virtual void quickSim(size_t ticks);
    virtual void tick(float dt, bool fast);
    virtual QString layoutName() = 0;
    System& system();
private:
    System mSystem;
};

Q_DECLARE_INTERFACE(LayoutPlugin,LayoutPlugin_iid)

#endif // LAYOUTPLUGIN_H
