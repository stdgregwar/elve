#include "Damp.h"
#include "Point.h"

namespace Elve {

Damp::Damp(qreal b) : mB(b)
{

}

void Damp::setB(qreal b) {
    mB = b;
}

QVector2D Damp::force(const Point &m) const
{
    return -mB*m.speed();
}

}
