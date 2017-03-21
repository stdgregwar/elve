#pragma once

#include <interfaces/NodeLook.h>

#include <QGraphicsRectItem>

class BlockNodeLook : public Elve::NodeLook
{
public:
    BlockNodeLook(const Elve::Node& node);
    QPointF inputPos(int index) const override;
    QPointF outputPos(int index) const override;
    void onColorChange(const QColor& col);
    void onColorReset();
private:
    QGraphicsRectItem* mMainRect;
};
