#include "BlockEdgeLook.h"
#include <QPainterPath>
#include <QPen>

using namespace Elve;

BasicEdgeLook::BasicEdgeLook(const NodeLook &from, const NodeLook &to) : EdgeLook(from,to)
{

}

QPen BasicEdgeLook::pen() {
    return QPen(Qt::white);
}

void BasicEdgeLook::addToPath(QPainterPath& p) {
    QPointF p1 = from().outputPos(iFrom());
    QPointF p2 = to().inputPos(iTo());

    p.moveTo(p1);
    qreal fac = 0.5;
    QPointF ctl1 = QPointF(p1.x()*(1-fac)+p2.x()*fac,p1.y());
    QPointF ctl2 = QPointF(p1.x()*(fac)+p2.x()*(1-fac),p2.y());
    p.cubicTo(ctl1,ctl2,p2);
}
