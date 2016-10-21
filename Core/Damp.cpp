#include "Damp.h"
#include "Mass.h"

Damp::Damp(qreal b) : mB(b)
{

}

QVector2D Damp::force(const Mass &m) const
{
    return -mB*m.speed();
}
