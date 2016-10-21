#ifndef SIMPLEFORCELAYOUT_H
#define SIMPLEFORCELAYOUT_H

#include <LayoutPolicy.h>

class SimpleForceLayout : public LayoutPolicy
{
public:
    SimpleForceLayout(QGraphicsScene& scene);
    void setGraph(SharedGraph graph, const NodePositions& positions) override;
};

#endif // SIMPLEFORCELAYOUT_H
