#include "NodeItem.h"
#include <QBrush>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QIcon>
#include <unordered_map>

#define SIZE 4

using namespace std;

NodeItem::NodeItem(const NodeID &id, const Node::Type &type, QGraphicsItem *parent) : QGraphicsPixmapItem(parent), mDraged(false), mId(id)
{
    setToolTip(QString(id.c_str()));
    static unordered_map<Node::Type,QString> pixmaps{{Node::CLUSTER,":/resources/cluster.svg"},{Node::NODE,":/resources/node.svg"},{Node::INPUT,":/resources/input.svg"},{Node::OUTPUT,":/resources/output.svg"}};

    setZValue(1);
    setPixmap(QIcon(pixmaps.at(type)).pixmap(QSize(64,64)));
    setOffset(-32,-32);
    setScale(0.25f);

    //setBrush(QBrush(QColor(240,240,240)));
    setAcceptedMouseButtons(Qt::MouseButton::AllButtons);
}

void NodeItem::onStateChange(const QVector2D& p, const QVector2D& speed)
{
    if(mDraged) {
        setState({pos().x(),pos().y()},{0,0});
    } else {
        setPos(p.x(),p.y());
    }
}


void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        mDraged = true;
        qDebug() << mId.c_str();
        event->accept();
    }
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        mDraged = false;
        QPointF delta = event->scenePos() - event->lastScenePos();
        setState({event->scenePos().x(),event->scenePos().y()},
        {delta.x(),delta.y()});
        event->accept();
    }
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mDraged) {
        setState({event->scenePos().x(),event->scenePos().y()},{0,0});
        setPos(event->scenePos());
        event->accept();
    }
}
