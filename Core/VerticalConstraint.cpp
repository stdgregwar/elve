#include "VerticalConstraint.h"
#include "Mass.h"

VerticalConstraint::VerticalConstraint(qreal height) : mHeight(height)
{

}


void VerticalConstraint::constrain(Mass &mass) {

    float hardn = 0.1;
    QVector2D p = mass.pos();
    QVector2D speed = mass.speed();

    mass.setPos({p.x(),mHeight*hardn+p.y()*(1-hardn)});
    mass.setSpeed({speed.x(),0});
}
