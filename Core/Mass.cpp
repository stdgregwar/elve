#include "Mass.h"
#include <QDebug>

Mass::Mass(qreal mass, Movable *m) : mM(mass), mContainerData(nullptr)
{
    mMovables.insert(m);
}

bool Mass::moved() const
{
    return true; //mSpeed.lengthSquared() > 1;
}

const QVector2D& Mass::pos() const
{
    return mPos;
}

void Mass::addMovable(Movable *m)
{
    mMovables.insert(m);
}

void Mass::removeMovable(Movable *m)
{
    mMovables.erase(m);
}

const QVector2D& Mass::speed() const
{
    return mSpeed;
}

const qreal& Mass::mass() const {
    return mM;
}

void Mass::addForce(Force *force) {
    mForces.insert(force);
}

void Mass::addConstraint(Constraint* c)
{
    mConstraints.insert(c);
}

void Mass::resetForce()
{
    mForce = QVector2D(0,0);
}

void Mass::computeForce()
{
    for(const Force* j : mForces) {
        QVector2D force = j->force(*this);
        if(true) {
            mForce += force;
        }
    }
}

void Mass::setPos(const QVector2D& pos)
{
    mPos = pos;
}

void Mass::setSpeed(const QVector2D& speed)
{
    mSpeed = speed;
}

void Mass::tick(float dt, bool update)
{
    QVector2D a = mForce / mM;

    mSpeed += a * dt;
    mPos += mSpeed*dt;

    for(Constraint* c : mConstraints) {
        c->constrain(*this);
    }

    if(moved() && update) {
        for(Movable* m : mMovables) {
            m->onStateChange(mPos,mSpeed);
        }
    }
}

void* Mass::containerData() const
{
    return mContainerData;
}
void Mass::setContainerData(void* data) const
{
    mContainerData = data;
}
