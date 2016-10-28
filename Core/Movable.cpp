#include "Movable.h"

#include "Point.h"

void Movable::setState(const QVector2D &pos, const QVector2D &speed){
    if(mMass) {
        mMass->setPos(pos);
        mMass->setSpeed(speed);
    }
}

Point* Movable::mass()
{
    return mMass;
}

void Movable::setMass(Point *m) {
    mMass = m;
}
