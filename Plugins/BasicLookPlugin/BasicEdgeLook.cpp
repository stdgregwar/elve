#include "BasicEdgeLook.h"
#include <QPainterPath>
#include <QPen>

using namespace Elve;

static const QColor base = QColor(128,128,128);

BasicEdgeLook::BasicEdgeLook(const NodeLook &from, Index outi, const NodeLook &to, Index ini) : EdgeLook(from,outi,to,ini)
{

}

const QColor& _tcol(const QColor& col, const QColor& def) {
    return col.isValid() ? col : def;
}

QPen BasicEdgeLook::pen() const {
    const QColor& c1 = _tcol(from().color(),base);
    const QColor& c2 = _tcol(to().color(),base);

    QPen p(QColor((c1.red()+c2.red())/2,
                       (c1.green()+c2.green())/2,
                       (c1.blue()+c2.blue())/2));
    p.setCosmetic(true);
    return p;
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
