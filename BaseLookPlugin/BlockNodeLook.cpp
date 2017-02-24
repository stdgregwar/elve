#include "BlockNodeLook.h"
#include <QBrush>
#include <QPen>
#include <QDebug>

static QColor baseColor(34,34,34);

BlockNodeLook::BlockNodeLook(const Node& node) : NodeLook(node)
{
    QPen p;
    p.setCosmetic(true);
    p.setColor(Qt::transparent);
    QBrush b(baseColor);

    size_t io_count = std::max(node.inputCount(),node.outputCount());

    mMainRect = new QGraphicsRectItem(0,0,70,io_count*20+20,this);
    addToGroup(mMainRect);

    mMainRect->setPen(p);
    mMainRect->setBrush(b);

    QBrush tb(Qt::white);
    QGraphicsSimpleTextItem* maintext = new QGraphicsSimpleTextItem(node.name().c_str(),mMainRect);
    maintext->setPos(5,5);
    maintext->setBrush(tb);
    addToGroup(maintext);

    QBrush ib(QColor(12,128,12));
    for(int i = 0; i < node.inputCount(); i++) {
        Name name = node.inputName(i);
        QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(name.c_str(),mMainRect);
        QGraphicsRectItem* rect = new QGraphicsRectItem(0,24+i*20,30,15,mMainRect);
        rect->setBrush(ib);
        rect->setPen(p);
        addToGroup(rect);
        text->setPos(5,20+i*20);
        addToGroup(text);
    }

    QBrush ob(QColor(128,12,12));
    for(int i = 0; i < node.outputCount(); i++) {
        Name name = node.outputName(i);
        QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(name.c_str(),mMainRect);
        text->setPos(40,20+i*20);
        QGraphicsRectItem* rect = new QGraphicsRectItem(40,24+i*20,30,15,mMainRect);
        rect->setBrush(ob);
        rect->setPen(p);
        addToGroup(rect);
        addToGroup(text);
    }
}

QPointF BlockNodeLook::inputPos(int index) const {
    return pos() + QPointF(0,30+20*index);
}

QPointF BlockNodeLook::outputPos(int index) const {
    return pos() + QPointF(70,30+20*index);
}

void BlockNodeLook::setColor(const QColor& col) {
    QPen p = mMainRect->pen();
    p.setColor(col);
    mMainRect->setPen(p);
}

void BlockNodeLook::resetColor() {
    QPen p = mMainRect->pen();
    p.setColor(Qt::transparent);
    mMainRect->setPen(p);
}
