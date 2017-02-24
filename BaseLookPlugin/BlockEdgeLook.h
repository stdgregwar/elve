#ifndef BLOCKEDGELOOK_H
#define BLOCKEDGELOOK_H

#include <interfaces/EdgeLook.h>

class BlockEdgeLook : public EdgeLook
{
public:
    BlockEdgeLook(const NodeLook& from, const NodeLook& to);
    void addToPath(QPainterPath& path) override;
    QPen pen() override;
};

#endif // BLOCKEDGELOOK_H
