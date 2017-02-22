#ifndef EDGELOOK_H
#define EDGELOOK_H

#include "NodeLook.h"

class EdgeLook
{
public:
    EdgeLook(NodeLook* from, NodeLook* to);
    virtual void addToPath(QPainterPath& path) = 0;
    NodeLook* from();
    NodeLook* to();
private:
    NodeLook* mAncestor;
    NodeLook* mChild;
};

#endif // EDGELOOK_H
