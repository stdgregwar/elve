#ifndef GRAVITY_H
#define GRAVITY_H

#include "Force.h"
#include "Point.h"

class Gravity : public Force
{
public:
    Gravity(qreal k);
    virtual QVector2D force(const Point &m) const override;
    QVector2D plainOldGravity(const Point& m) const;
    void addMass(const Point* m);
    void clear();
private:
    std::set<const Point*> mGalaxy;
    qreal mK;
};

#endif // GRAVITY_H
