#ifndef JEDI_H
#define JEDI_H

#include <QVector2D>

namespace Elve {

class Point;

class Force
{
public:
    virtual QVector2D force(const Point& m) const = 0;
    virtual ~Force(){}
};

}
#endif // JEDI_H
