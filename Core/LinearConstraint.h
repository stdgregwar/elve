#pragma once

#include "Constraint.h"
#include "QVector2D"
#include "functional"

class LinearConstraint : public Constraint
{

public:
    enum Dir{
        VERTICAL,
        HORIZONTAL
    };

    LinearConstraint(qreal position, qreal hardness,Dir direction);
    void constrain(Point &mass) override;
    void constrainVert(Point & mass);
    void constrainHor(Point &mass);
private:
    std::function<void(LinearConstraint*,Point& mass)> mConst;
    qreal mPosition;
    qreal mHardness;
};
