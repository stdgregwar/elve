#ifndef NODELOOK_H
#define NODELOOK_H

#include <QGraphicsItem>
#include <Node.h>
#include <Movable.h>

class NodeLook : public QGraphicsItem, public Movable
{
public:
    NodeLook(Node* n);

    virtual QPointF inputPos(int index);
    virtual QPointF outputPos(int index);
    virtual QPointF inputPos(const NodeID& from);
    virtual QPointF outputPos(const NodeID& to);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void onStateChange(const QVector2D& pos, const QVector2D& speed) override;


    Node* node();
private:
    Node* mNode;
    bool mDraged;
};

#endif // NODELOOK_H
