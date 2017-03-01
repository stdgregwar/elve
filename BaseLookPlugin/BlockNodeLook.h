#pragma once

#include <NodeLook.h>

#include <QGraphicsRectItem>

class BlockNodeLook : public Elve::NodeLook
{
public:
    BlockNodeLook(const Elve::Node& node);
    QPointF inputPos(int index) const override;
    QPointF outputPos(int index) const override;
    void setColor(const QColor& col);
    void resetColor();
private:
    QGraphicsRectItem* mMainRect;
};
