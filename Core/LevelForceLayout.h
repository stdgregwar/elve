#ifndef LEVELFORCELAYOUT_H
#define LEVELFORCELAYOUT_H

#include <LayoutPolicy.h>

class LevelForceLayout : public LayoutPolicy
{
public:
    LevelForceLayout(QGraphicsScene& scene);
    void setGraph(SharedGraph graph, const NodePositions& positions) override;
};

#endif // LEVELFORCELAYOUT_H
