#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include "Movable.h"
#include "Node.h"

class NodeItem : public QGraphicsPixmapItem, public Movable
{
public:
    NodeItem(const NodeID& id,const Node::Type& type,QGraphicsItem* parent = nullptr);
    void onStateChange(const QVector2D& pos, const QVector2D& speed) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    const NodeID& id() const {return mId;}
private:
    bool mDraged;
    NodeID mId;
};

#endif // NODEITEM_H
