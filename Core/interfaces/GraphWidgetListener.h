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
    virtual void graphChanged(SharedEGraph oldGraph, SharedEGraph newGraph) = 0;
    virtual void runCommand(const QString& cmd) = 0;
};

class DummyListener : public GraphWidgetListener {
    void graphChanged(SharedEGraph oldGraph, SharedEGraph newGraph) override {}
    void runCommand(const QString& cmd) override {}
};

}
#endif // GRAPHWIDGETLISTENER_H
