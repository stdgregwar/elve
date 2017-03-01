#ifndef VERTICALCONSTRAINT_H
#define VERTICALCONSTRAINT_H

#include "Constraint.h"
#include <QVector2D>
namespace Elve {
class VerticalConstraint : public Constraint
{
public:
    VerticalConstraint(qreal height);
    void constrain(Point &mass) override;
private:
    qreal mHeight;
};
}
#endif // VERTICALCONSTRAINT_H
