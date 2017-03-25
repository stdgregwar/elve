#include "NodeLook.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>

#include "Point.h"
namespace Elve {
NodeLook::NodeLook(const Node &n) : mNode(n), mDraged(false)
{

}

void NodeLook::onStateChange(const QVector2D& p, const QVector2D& speed)
{
    if(mDraged) {
        QPointF np = pos();
        setState({np.x(),np.y()},{0,0});
    } else {
        setPos(p.toPointF());
    }
}

void NodeLook::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        mDraged = true;
        mOffset = pos() - event->scenePos();
        event->accept();
    }
}

void NodeLook::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        mDraged = false;
        QPointF delta = event->scenePos() - event->lastScenePos();
        QPointF rpos = event->scenePos();
        setState({rpos.x(),rpos.y()},
        {delta.x(),delta.y()});
        bool pinned = point()->pinned();

        if(event->modifiers().testFlag(Qt::ControlModifier)) {
            pinned ? point()->unpin() : point()->pin();
        } else if(pinned) {
            point()->pin();
        }
        event->accept();
    }
}

void NodeLook::color(const QColor& color) {
    mSelectionColor = color;
    color.isValid() ? onColorChange(color) : onColorReset();
}

const QColor& NodeLook::color() const {
    return mSelectionColor;
}

void NodeLook::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mDraged) {
        setState({event->scenePos().x()+mOffset.x(),event->scenePos().y()+mOffset.y()},{0,0});
        setPos(event->scenePos()+mOffset);
        event->accept();
    }
}

QPointF NodeLook::inputPos(int index) const {
    return pos();
}

QPointF NodeLook::outputPos(int index) const {
    return pos();
}

const Node &NodeLook::node() const {
    return mNode;
}
}
