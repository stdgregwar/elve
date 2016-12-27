#include "Movable.h"

#include "Point.h"

void Movable::setState(const QVector2D &pos, const QVector2D &speed){
    if(mPoint) {
        mPoint->setPos(pos);
        mPoint->setSpeed(speed);
        for(Movable* m : mPoint->movables()) {
            if(m != this) {
                m->onStateChange(pos,speed);
            }
        }
    }
}

Point* Movable::point()
{
    return mPoint;
}

void Movable::setPoint(Point *m) {
    mPoint = m;
}
