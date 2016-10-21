#ifndef JEDI_H
#define JEDI_H

#include <QVector2D>

class Mass;

class Force
{
public:
    virtual QVector2D force(const Mass& m) const = 0;
    virtual ~Force(){}
};

#endif // JEDI_H
