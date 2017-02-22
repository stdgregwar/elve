#include "BlockNodeLook.h"
#include <QBrush>
#include <QPen>
#include <QDebug>

BlockNodeLook::BlockNodeLook(const Node& node) : NodeLook(node)
{
    QPen p;
    p.setWidth(0);
    QBrush b(QColor(128,128,128));

    size_t io_count = std::max(node.inputCount(),node.outputCount());

    qDebug() << "inputs " << node.inputCount() << " outputs " << node.outputCount();

    mMainRect = new QGraphicsRectItem(0,0,200,io_count*20+20,this);
    addToGroup(mMainRect);

    mMainRect->setPen(p);
    mMainRect->setBrush(b);

    addToGroup(new QGraphicsSimpleTextItem(node.name().c_str(),mMainRect));
}

QPointF BlockNodeLook::inputPos(int index) {

}

QPointF BlockNodeLook::outputPos(int index) {

}

void BlockNodeLook::setColor(const QColor& col) {
    QBrush b = mMainRect->brush();
    b.setColor(col);
    mMainRect->setBrush(b);
}

void BlockNodeLook::resetColor() {
    //Todo
}
