#include "Point.h"
#include <QDebug>

#include "System.h"
#include "PointConstraint.h"

#define MAX_FORCE 1e14

namespace Elve {

Point::Point(qreal mass, const NodeID &id, System& sys) : mM(mass), mContainerData(nullptr), mID(id), mSys(sys)
{
    //mMovables.insert(id);
}

bool Point::moved() const
{
    return true; //mSpeed.lengthSquared() > 1;
}

const QVector2D& Point::pos() const
{
    return mPos;
}

void Point::addMovable(Movable *m)
{
    m->setPoint(this);
    mMovables.insert(m);
}

void Point::removeMovable(Movable *m)
{
    mMovables.erase(m);
}

const QVector2D& Point::speed() const
{
    return mSpeed;
}

const qreal& Point::mass() const {
    return mM;
}

void Point::setMass(qreal mass) {
    mM = mass;
}

void Point::addForce(Force *force) {
    mForces.insert(force);
}

void Point::clearContraints() {
    mConstraints.clear();
}

void Point::addConstraint(Constraint* c)
{
    mConstraints.insert(c);
}

void Point::resetForce()
{
    mForce = QVector2D(0,0);
}

void Point::computeForce()
{
    for(const Force* j : mForces) {
        QVector2D force = j->force(*this);
        if(force.length() < MAX_FORCE) {
            mForce += force;
        }
    }
}

const NodeID& Point::boundID() const {
    return mID;
}

void Point::setPos(const QVector2D& pos)
{
    mPos = pos;
}

void Point::setSpeed(const QVector2D& speed)
{
    mSpeed = speed;
}

void Point::tick(float dt, bool update)
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

void Point::removePConstraints() {
    for(auto it = mConstraints.begin(); it != mConstraints.end(); it++) {
        PointConstraint* pc = dynamic_cast<PointConstraint*>(*it);
        if(pc) {
            delete *it;
            mConstraints.erase(it);
        }
    }
}

bool Point::pinned() const {
    return mSys.pinnedPoints().count(mID);
}

void Point::pin() {
    mSys.pin(mID,pos());
}

void Point::unpin() {
    mSys.unpin(mID);
}

void Point::clearMovables() {
    mMovables.clear();
}

void* Point::containerData() const
{
    return mContainerData;
}
void Point::setContainerData(void* data) const
{
    mContainerData = data;
}

}
