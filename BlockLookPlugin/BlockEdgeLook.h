#ifndef BLOCKEDGELOOK_H
#define BLOCKEDGELOOK_H

#include <interfaces/EdgeLook.h>

class BasicEdgeLook : public Elve::EdgeLook
{
public:
    BasicEdgeLook(const Elve::NodeLook& from, const Elve::NodeLook& to);
    void addToPath(QPainterPath& path) override;
    QPen pen() override;
};

#endif // BLOCKEDGELOOK_H
