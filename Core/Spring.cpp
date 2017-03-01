#include "Spring.h"
#include "Point.h"
#include <QDebug>

namespace Elve {

Spring::Spring(Point& first, Point& second, qreal l0, qreal k) : m1(first), m2(second), mL0(l0), mK(k)
{

}

QVector2D Spring::force(const Point &m) const {
    QVector2D r = (m1.pos() - m2.pos());
    qreal l = r.length();
    QVector2D f = r.normalized() * mK * (l-mL0);
    if(&m == &m1) {
        return -f;
    }
    return f;
}

}
