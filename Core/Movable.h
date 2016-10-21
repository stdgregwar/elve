#ifndef MOVABLE_H
#define MOVABLE_H

#include <QVector2D>

class Mass;

class Movable
{
public:
    virtual void onStateChange(const QVector2D& pos, const QVector2D& speed) = 0;
    void setState(const QVector2D& pos, const QVector2D& speed);
    void setMass(Mass* m);
    Mass* mass();
private:
    Mass* mMass;
};

#endif // MOVABLE_H
