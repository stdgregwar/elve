#ifndef BOXCONSTRAINT_H
#define BOXCONSTRAINT_H

#include <QRectF>

#include "Constraint.h"

namespace Elve {

class BoxConstraint : public Constraint
{
public:
    BoxConstraint(const QRectF& bounds = {0,0,1024,1024});
    void setBounds(const QRectF& bounds) ;
    void constrain(Point& point) override;
private:
    QRectF mBounds;
};

}
#endif // BOXCONSTRAINT_H
