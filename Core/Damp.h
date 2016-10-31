#ifndef DAMP_H
#define DAMP_H

#include "Force.h"


class Damp : public Force
{
public:
    Damp(qreal b);
    QVector2D force(const Point &m) const override;
private:
    qreal mB;
};

#endif // DAMP_H
