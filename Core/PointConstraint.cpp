#include "PointConstraint.h"
#include "Mass.h"


PointConstraint::PointConstraint(const QVector2D& point) : mPoint(point)
{

}

void PointConstraint::constrain(Mass& mass)
{
    mass.setPos(mPoint);
    mass.setSpeed({0,0});
}
