#ifndef MOVABLE_H
#define MOVABLE_H

#include <QVector2D>

class Point;

class Movable
{
public:
    virtual void onStateChange(const QVector2D& pos, const QVector2D& speed) = 0;
    void setState(const QVector2D& pos, const QVector2D& speed);
    void setPoint(Point* m);
    Point* point();
private:
    Point* mPoint;
};

#endif // MOVABLE_H
