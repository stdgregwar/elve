#ifndef GRAPHWIDGETLISTENER_H
#define GRAPHWIDGETLISTENER_H

#include <ExtendedGraph.h>

namespace Elve {

/**
 * @brief The GraphWidgetListener interface reimplement this if you're using elve
 * as a dependency
 *
 */
class GraphWidgetListener
{
public:
    GraphWidgetListener();
    virtual void graphChanged(const SharedEGraph& oldGraph, const SharedEGraph& newGraph) = 0;
    virtual void selectionChanged(const SharedEGraph& graph) = 0;
    virtual void runCommand(const QString& cmd) = 0;
};

class DummyListener : public GraphWidgetListener {
    void graphChanged(const SharedEGraph&, const SharedEGraph&) override {}
    virtual void selectionChanged(const SharedEGraph&) override {}
    void runCommand(const QString&) override {}
};

}
#endif // GRAPHWIDGETLISTENER_H
