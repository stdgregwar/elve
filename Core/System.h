#ifndef SYSTEM_H
#define SYSTEM_H

#include "Mass.h"
#include "Spring.h"
#include "Force.h"
#include "Gravity.h"
#include "Constraint.h"
#include <vector>

enum GravityMode {
    FULL,
    GHOST,
    NONE
};

class System
{
public:
    System();
    void tick(float dt,bool update = true);
    Mass* addMass(qreal mass, Movable* m, QVector2D pos, qreal damp, GravityMode g = FULL);
    void addSpring(unsigned i, unsigned j, qreal k, qreal l0);
    void addSpring(Mass* mi, Mass* mj, qreal k, qreal l0);
    void addVConstraint(Mass* m, qreal height);
    void addPConstrain(Mass* m, const QVector2D& p);
    void clear();
    size_t massCount() const;
    size_t forceCount() const;
    const Mass* nearest(const QVector2D& p) const;
    ~System();
private:
    void computeForces(size_t from,size_t until);
    Gravity mGravity;
    std::vector<Mass*> mMasses;
    std::vector<Force*> mForces;
    std::vector<Constraint*> mConstraints;
};

#endif // SYSTEM_H
