#ifndef GRAPHWIDGETLISTENER_H
#define GRAPHWIDGETLISTENER_H

#include <EGraph.h>
namespace Elve {

class GraphWidgetListener
{
public:
    GraphWidgetListener();
    virtual void graphChanged(SharedEGraph oldGraph, SharedEGraph newGraph){}
};

}
#endif // GRAPHWIDGETLISTENER_H
