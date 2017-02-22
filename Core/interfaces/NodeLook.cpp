#include "NodeLook.h"

#include <QGraphicsSceneMouseEvent>

NodeLook::NodeLook(const Node &n) : mNode(n), mDraged(false)
{

}

void NodeLook::onStateChange(const QVector2D& p, const QVector2D& speed)
{
    if(mDraged) {
        QPointF np = pos() * inverseOrientationTransform();
        setState({np.x(),np.y()},{0,0});
    } else {
        setPos(p.toPointF() * orientationTransform());
    }
}

void NodeLook::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        mDraged = true;
        event->accept();
    }
}

void NodeLook::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        mDraged = false;
        QPointF delta = event->scenePos() - event->lastScenePos();
        setState({event->scenePos().x(),event->scenePos().y()},
        {delta.x(),delta.y()});
        event->accept();
    }
}

void NodeLook::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mDraged) {
        setState({event->scenePos().x(),event->scenePos().y()},{0,0});
        //setPos(event->scenePos());
        event->accept();
    }
}

QPointF NodeLook::inputPos(int index) {
    return pos();
}

QPointF NodeLook::outputPos(int index) {
    return pos();
}

QPointF NodeLook::inputPos(const NodeID& from) {
    int i = 0;
    for(Node* n : mNode.ancestors()) {
        if(n->id() == from) {
            return inputPos(i);
        }
        i++;
    }
    return inputPos(0);
}

QPointF NodeLook::outputPos(const NodeID& to) {
    int i = 0;
    for(Node* n : mNode.children()) {
        if(n->id() == to) {
            return outputPos(i);
        }
        i++;
    }
    return inputPos(0);
}

const Node &NodeLook::node() const {
    return mNode;
}

const QTransform& NodeLook::orientationTransform() const {
    return *mTransform;
}

const QTransform& NodeLook::inverseOrientationTransform() const {
    return *mInverseTransform;
}

void NodeLook::setOrientationTransform(const QTransform *transform, const QTransform *inverse) {
    mTransform = transform;
    mInverseTransform = inverse;
}
