#ifndef NODELOOK_H
#define NODELOOK_H

#include <QGraphicsItemGroup>
#include <Node.h>
#include <Movable.h>

enum OrientationHint {
    BOTTOMUP,
    TOPDOWN,
    LEFTRIGHT,
    RIGHTLEFT
};

class NodeLook : public QGraphicsItemGroup, public Movable
{
public:
    NodeLook(const Node& n);

    //To override when creating looks
    virtual QPointF inputPos(int index);
    virtual QPointF outputPos(int index);
    virtual QPointF inputPos(const NodeID& from);
    virtual QPointF outputPos(const NodeID& to);
    virtual void setColor(const QColor& col) = 0;
    virtual void resetColor() = 0;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void onStateChange(const QVector2D& pos, const QVector2D& speed) override;
    const Node& node() const;

    const QTransform& orientationTransform() const;
    const QTransform& inverseOrientationTransform() const;
    void setOrientationTransform(const QTransform* transform, const QTransform* inverse);
private:
    const Node& mNode;
    const QTransform* mTransform;
    const QTransform* mInverseTransform;
    bool mDraged;
};

#endif // NODELOOK_H
