#include "BoxConstraint.h"
#include "Point.h"

namespace Elve {
using namespace std;

BoxConstraint::BoxConstraint(const QRectF &bounds) : mBounds(bounds)
{

}

void BoxConstraint::setBounds(const QRectF& bounds) {
    mBounds = bounds;
}


void BoxConstraint::constrain(Point& point) {
    QVector2D pos = point.pos();
    pos.setX(max(mBounds.left(),min(mBounds.right(),(qreal)pos.x())));
    pos.setY(max(mBounds.top(),min(mBounds.bottom(),(qreal)pos.y())));
    point.setPos(pos);
}

}
