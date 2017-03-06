#pragma once

#include <NodeLook.h>

#include <QGraphicsRectItem>

class BasicNodeLook : public Elve::NodeLook
{
public:
    BasicNodeLook(const Elve::Node& node);
    QPointF inputPos(int index) const override;
    QPointF outputPos(int index) const override;
    void setColor(const QColor& col);
    void resetColor();
private:
    QGraphicsRectItem* mMainRect;
};
