#ifndef NODELOOK_H
#define NODELOOK_H

#include <QGraphicsItemGroup>
#include <Node.h>
#include <Movable.h>

namespace Elve {

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
    virtual QPointF inputPos(int index) const ;
    virtual QPointF outputPos(int index) const ;
    virtual void onColorChange(const QColor& col) = 0;
    virtual void onColorReset() = 0;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void onStateChange(const QVector2D& pos, const QVector2D& speed) override;

    void color(const QColor& color);
    const QColor& color() const;
    const Node& node() const;
private:
    const Node& mNode;
    QPointF mOffset;
    QColor  mSelectionColor;
    bool mDraged;
};
}
#endif // NODELOOK_H
