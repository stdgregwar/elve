#include "PointConstraint.h"
#include "Point.h"

namespace Elve {

PointConstraint::PointConstraint(const QVector2D& point) : mPoint(point)
{

}

void PointConstraint::constrain(Point& mass)
{
    mass.setPos(mPoint);
    mass.setSpeed({0,0});
}

}
