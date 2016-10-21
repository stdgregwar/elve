#include "Movable.h"

#include "Mass.h"

void Movable::setState(const QVector2D &pos, const QVector2D &speed){
    if(mMass) {
        mMass->setPos(pos);
        mMass->setSpeed(speed);
    }
}

Mass* Movable::mass()
{
    return mMass;
}

void Movable::setMass(Mass *m) {
    mMass = m;
}
