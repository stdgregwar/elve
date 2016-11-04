#include "EdgeItem.h"
#include <QPainterPath>
#include <QBrush>
#include <QPen>

EdgeItem::EdgeItem(size_t segments)
{
    mHandles.resize(segments+1,this);
    /*QPen p(QColor(100,100,100), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    p.setCosmetic(true);
    setPen(p);*/
}

void EdgeItem::setHandleState(size_t i, const QVector2D& pos, const QVector2D& speed) {
    mHandles[i].pos() = pos;
    //doPath();
}

size_t EdgeItem::segments() const
{
    return mHandles.size()-1;
}

Movable* EdgeItem::getHandle(size_t i)
{
    return &mHandles[i];
}

void EdgeItem::handleMoved()
{
    //doPath();
}

void EdgeItem::doPath(QPainterPath& p) {
    p.moveTo(mHandles[0].pos().toPointF());
    /*for(size_t i = 1; i < mHandles.size()-2; i++) {
        p.cubicTo(mHandles[i].pos(),
                  mHandles[i+1].pos(),
                mHandles[i+2].pos());

    }*/

    for(size_t i = 1; i < mHandles.size(); i++) {
        p.lineTo(mHandles[i].pos().toPointF());
        //sp.addEllipse(mHandles[i].pos(),8,8);
    }
    //p.addPath(sp);
    //setPath(p);
}
