#ifndef CONSTRAINT_H
#define CONSTRAINT_H

class Mass;

class Constraint
{
public:
    virtual void constrain(Mass& mass) = 0;
};

#endif // CONSTRAINT_H
