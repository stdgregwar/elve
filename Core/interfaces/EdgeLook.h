#ifndef EDGELOOK_H
#define EDGELOOK_H

#include "NodeLook.h"

class EdgeLook
{
public:
    EdgeLook(NodeLook* ancestor, NodeLook* child);
    virtual void addToPath(QPainterPath& path) = 0;
    NodeLook* ancestor();
    NodeLook* child();
private:
    NodeLook* mAncestor;
    NodeLook* mChild;
};

#endif // EDGELOOK_H
