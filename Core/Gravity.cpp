#include "Gravity.h"
#include <QDebug>
#include <QGraphicsScene>


#define QSIZE 32000

Gravity::Gravity(qreal k) : mK(k) ,mQuadTree(QRectF(-QSIZE,-QSIZE,QSIZE*2,QSIZE*2))
{

}

QVector2D Gravity::force(const Point &m) const {
   return plainOldGravity(m);
   //return quadGravity(m);
}

void Gravity::addPoint(const Point* m)
{
    mGalaxy.insert(m);
    //mQuadTree.addPoint(m);
}

QVector2D Gravity::plainOldGravity(const Point& m) const
{
    QVector2D f{0,0};
    for(const Point* const& ms : mGalaxy) {
        if(ms != &m) {
            QVector2D r = ms->pos() - m.pos();
            qreal lr = r.lengthSquared();
            if(lr > 1e-2 and lr < 1024*1024) {
                f += -r.normalized() * ((mK*ms->mass()*m.mass()) / lr);
            }
        }
    }
    return f;
}

void Gravity::updateQuadTree() {
    /*for(const Point* m : mGalaxy) {
        if(m->moved()) {
            mQuadTree.movePoint(m);
        }
    }*/
    //mQuadTree.reinsertAll();
}

QVector2D Gravity::quadGravity(const Point& m) const
{
    return mK*mQuadTree.gravityFor(m);
    //return plainOldGravity(m);
}

void Gravity::debug(QPainter* p) const {
    mQuadTree.debug(p);
}

void Gravity::clear() {
    mGalaxy.clear();
    mQuadTree.clear();
}
