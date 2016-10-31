#include "Movable.h"

#include "Point.h"

void Movable::setState(const QVector2D &pos, const QVector2D &speed){
    if(mMass) {
        mMass->setPos(pos);
        mMass->setSpeed(speed);
    }
}

Point* Movable::point()
{
    return mMass;
}

void Movable::setPoint(Point *m) {
    mMass = m;
}
