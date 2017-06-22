#pragma once

#include <interfaces/EdgeLook.h>

class BasicEdgeLook : public Elve::EdgeLook
{
public:
    BasicEdgeLook(const Elve::NodeLook& from,Elve::Index outi, const Elve::NodeLook& to, Elve::Index ini);
    void addToPath(QPainterPath& path) override;
    QPen pen() const override;
};
