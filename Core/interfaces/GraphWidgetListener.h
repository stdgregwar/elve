#ifndef GRAPHWIDGETLISTENER_H
#define GRAPHWIDGETLISTENER_H

#include <ExtendedGraph.h>
namespace Elve {

class GraphWidgetListener
{
public:
    GraphWidgetListener();
    virtual void graphChanged(SharedEGraph oldGraph, SharedEGraph newGraph){}
};

}
#endif // GRAPHWIDGETLISTENER_H
