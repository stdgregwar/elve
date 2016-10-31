#include "Damp.h"
#include "Point.h"

Damp::Damp(qreal b) : mB(b)
{

}

QVector2D Damp::force(const Point &m) const
{
    return -mB*m.speed();
}
