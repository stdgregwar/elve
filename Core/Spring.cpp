#include "Spring.h"
#include "Mass.h"
#include <QDebug>
Spring::Spring(Mass& first, Mass& second, qreal l0, qreal k) : m1(first), m2(second), mL0(l0), mK(k)
{

}

QVector2D Spring::force(const Mass &m) const {
    QVector2D r = (m1.pos() - m2.pos());
    qreal l = r.length();
    QVector2D f = r.normalized() * mK * (l-mL0);
    if(f.x() != f.x() or f.y() != f.y()) {
        qDebug() << "NAN!";
    }
    if(&m == &m1) {
        return -f;
    }
    return f;
}
