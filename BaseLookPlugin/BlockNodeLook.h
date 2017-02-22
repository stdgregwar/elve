#pragma once

#include <NodeLook.h>

#include <QGraphicsRectItem>

class BlockNodeLook : public NodeLook
{
public:
    BlockNodeLook(const Node& node);
    QPointF inputPos(int index);
    QPointF outputPos(int index);
    void setColor(const QColor& col);
    void resetColor();
private:
    QGraphicsRectItem* mMainRect;
};
