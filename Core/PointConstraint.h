#ifndef POINTCONSTRAINT_H
#define POINTCONSTRAINT_H

#include "Constraint.h"
#include <QVector2D>

class PointConstraint : public Constraint
{
public:
    PointConstraint(const QVector2D& point);
    void constrain(Mass& mass) override;
private:
    QVector2D mPoint;
};

#endif // POINTCONSTRAINT_H
