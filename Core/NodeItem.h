#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include "Movable.h"
#include "Node.h"

class NodeItem : public QGraphicsPixmapItem, public Movable
{
public:
    NodeItem(const NodeData& data,QGraphicsItem* parent = nullptr);
    void onStateChange(const QVector2D& pos, const QVector2D& speed) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    const NodeID& id() const {return mData.id();}
private:
    bool mDraged;
    const NodeData& mData;
};

#endif // NODEITEM_H
