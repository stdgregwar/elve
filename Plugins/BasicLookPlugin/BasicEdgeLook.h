#pragma once

#include <interfaces/EdgeLook.h>

class BasicEdgeLook : public Elve::EdgeLook
{
public:
    BasicEdgeLook(const Elve::NodeLook& from, const Elve::NodeLook& to);
    void addToPath(QPainterPath& path) override;
    QPen pen() const override;
};
