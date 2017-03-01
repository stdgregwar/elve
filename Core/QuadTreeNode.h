#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <set>
#include "Point.h"
#include <QRectF>
#include <QVector2D>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>

namespace Elve {

typedef std::vector<Point*> Masses;

struct QuadTreeParams {
    qreal theta;
    size_t depth;
    size_t maxMasses;
};

class QuadTreeNode
{
public:
    QuadTreeNode();
    void setCenterAndRadius(const QVector2D& pos, float radius);
    void setChild(unsigned index, QuadTreeNode* node);
    void setParent(QuadTreeNode* parent);

    bool addPoint(const Point *obj, const QuadTreeParams& params);
    void remPoint(const Point* obj, const QuadTreeParams& params);

    QVector2D gravityFor(const Point &m, const QuadTreeParams& params) const;
    bool leaf() const;

    void reset();
    bool contain(const Point *o) const;
    const QVector2D& getCenter() const;
    QVector2D CoM() const;
    const qreal& getRadius() const;
    const qreal& mass() const;
    const bool& downOccupied() const;
    void debug(QPainter* p) const;
    virtual ~QuadTreeNode();
private:
    std::vector<const Point *> &givePoints();
    QVector2D trueGravity(const Point& m) const;


    QuadTreeNode* mParent;
    QuadTreeNode* mChildren[4];
    std::vector<const Point*> mPoints;
    QVector2D mCenter;
    qreal mRadius;
    qreal mMass;
    size_t mCount;
    QVector2D mMC; ///Center of mass (not divided)
};

}
#endif // QUADTREENODE_H
