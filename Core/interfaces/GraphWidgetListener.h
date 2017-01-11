#ifndef GRAPHWIDGETLISTENER_H
#define GRAPHWIDGETLISTENER_H

#include <EGraph.h>

class GraphWidgetListener
{
public:
    GraphWidgetListener();
    virtual void graphChanged(SharedEGraph oldGraph, SharedEGraph newGraph){}
};

#endif // GRAPHWIDGETLISTENER_H
