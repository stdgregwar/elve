#pragma once

#include <NodeLook.h>

#include <QGraphicsSvgItem>

#include "BasicLookPlugin.h"

class BasicNodeLook : public Elve::NodeLook
{
public:
    BasicNodeLook(BasicLookPlugin& look,const Elve::Node& node);
    //QPointF inputPos(int index) const override;
    //QPointF outputPos(int index) const override;
    void setColor(const QColor& col);
    void resetColor();
private:
    QGraphicsSvgItem* mItem;
    BasicLookPlugin& mLook;
};
