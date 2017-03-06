#include "LinearConstraint.h"
#include "Point.h"

namespace Elve {

LinearConstraint::LinearConstraint(qreal position, qreal hardness, Dir direction) : mPosition(position), mHardness(hardness)
{
    mConst = direction == VERTICAL ? &LinearConstraint::constrainVert : &LinearConstraint::constrainHor;
}

void LinearConstraint::constrain(Point &mass) {
    mConst(this,mass);
}

void LinearConstraint::constrainVert(Point & mass) {
    QVector2D p = mass.pos();
    QVector2D speed = mass.speed();

    mass.setPos({p.x(),mPosition*mHardness+p.y()*(1-mHardness)});
    mass.setSpeed({speed.x(),0});
}

void LinearConstraint::constrainHor(Point &mass) {
    QVector2D p = mass.pos();
    QVector2D speed = mass.speed();

    mass.setPos({mPosition*mHardness+p.x()*(1-mHardness),p.y()});
    mass.setSpeed({0,speed.y()});
}

}
