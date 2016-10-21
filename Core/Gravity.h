#ifndef GRAVITY_H
#define GRAVITY_H

#include "Force.h"
#include "Mass.h"

class Gravity : public Force
{
public:
    Gravity(qreal k);
    virtual QVector2D force(const Mass &m) const override;
    QVector2D plainOldGravity(const Mass& m) const;
    void addMass(const Mass* m);
    void clear();
private:
    std::set<const Mass*> mGalaxy;
    qreal mK;
};

#endif // GRAVITY_H
