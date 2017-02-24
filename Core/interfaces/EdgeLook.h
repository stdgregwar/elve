#ifndef EDGELOOK_H
#define EDGELOOK_H

#include "NodeLook.h"

class EdgeLook
{
public:
    EdgeLook(const NodeLook& from,const NodeLook& to);
    virtual void addToPath(QPainterPath& path) = 0;
    virtual QPen pen() = 0;
    const NodeLook& from() const;
    const NodeLook& to() const;
    Index iFrom() const;
    Index iTo() const;
private:
    const NodeLook& mFrom;
    const NodeLook& mTo;
    Index mIFrom;
    Index mITo;
};

#endif // EDGELOOK_H
