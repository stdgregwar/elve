#include "BasicEdgeLook.h"
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
    QPointF ctl1 = QPointF(from().x(),from().y()*(1-fac)+to().y()*fac);
    QPointF ctl2 = QPointF(to().x(),from().y()*(fac)+to().y()*(1-fac));
    p.cubicTo(ctl1,ctl2,p2);
}
