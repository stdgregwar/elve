#ifndef SPRING_H
#define SPRING_H

#include <QVector2D>
#include "Force.h"

class Point;

class Spring : public Force
{
public:
    Spring(Point& first, Point& second, qreal l0, qreal k);
    virtual QVector2D force(const Point &m) const override;
private:
    qreal mK;
    qreal mL0;
    Point& m1;
    Point& m2;
};

#endif // SPRING_H
