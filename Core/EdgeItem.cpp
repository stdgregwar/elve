#include "EdgeItem.h"
#include <QPainterPath>
#include <QBrush>
#include <QPen>

namespace Elve {

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
        QPointF from = mHandles[i-1].pos().toPointF();
        QPointF to = mHandles[i].pos().toPointF();

        qreal fac = 0.5;
        QPointF ctl1 = QPointF(from.x(),from.y()*(1-fac)+to.y()*fac);
        QPointF ctl2 = QPointF(to.x(),from.y()*(fac)+to.y()*(1-fac));
        p.cubicTo(ctl1,ctl2,to);
        //p.lineTo(mHandles[i].pos().toPointF());
        //p.cubicTo();
        //sp.addEllipse(mHandles[i].pos(),8,8);
    }
    //p.addPath(sp);
    //setPath(p);
}

}
