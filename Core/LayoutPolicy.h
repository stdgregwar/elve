#ifndef LAYOUTPOLICY_H
#define LAYOUTPOLICY_H

#include "System.h"
#include "Graph.h"
#include "EdgeItem.h"
#include "NodeItem.h"

typedef std::unordered_map<NodeID,Mass*> MassesByID;
typedef std::unordered_map<NodeID,QVector2D> NodePositions;

class LayoutPolicy
{
public:
    LayoutPolicy(QGraphicsScene& scene);
    virtual void setGraph(SharedGraph graph);
    virtual void setGraph(SharedGraph g,const NodePositions& positions) = 0;
    const MassesByID& massesByID() const;
    const System& system() const;
    virtual void quickSim(size_t ticks);
    virtual void tick(float dt, bool update = true);
    virtual void clear();
    NodePositions nodesPositions() const;
protected:
    EdgeItem* newEdge(size_t segments);
    NodeItem* newNode(const NodeID &id, const Node::Type& t);

    std::vector<EdgeItem*> mEdges;
    std::vector<NodeItem*> mNodes;
    System mSystem;
    MassesByID mMasses;
    QGraphicsScene& mScene;
};

#endif // LAYOUTPOLICY_H
