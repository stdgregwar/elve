#ifndef CONSTRAINT_H
#define CONSTRAINT_H

class Point;

class Constraint
{
public:
    virtual void constrain(Point& mass) = 0;
};

#endif // CONSTRAINT_H
