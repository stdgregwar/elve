#ifndef SPRING_H
#define SPRING_H

#include <QVector2D>
#include "Force.h"

class Mass;

class Spring : public Force
{
public:
    Spring(Mass& first, Mass& second, qreal l0, qreal k);
    virtual QVector2D force(const Mass &m) const override;
private:
    qreal mK;
    qreal mL0;
    Mass& m1;
    Mass& m2;
};

#endif // SPRING_H
