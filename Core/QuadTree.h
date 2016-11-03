#ifndef QUADTREE_H
#define QUADTREE_H

#include <cmath>
#include "QuadTreeNode.h"
#include "Point.h"
#include <QRectF>
#include <QVector2D>
#include <QGraphicsItem>

constexpr unsigned long QUADTREESIZE = (1UL+2UL*2UL+4UL*4UL+8UL*8UL+16UL*16UL+32UL*32UL+64UL*64UL+128UL*128UL);

class QuadTree
{
public:
    QuadTree(const QRectF& bounds);
    QuadTree& operator=(QuadTree&& other);
    bool addPoint(const Point* m);
    void removePoint(const Point* r);
    void movePoint(const Point* r);

    void reinsertAll();

    QVector2D gravityFor(const Point &m) const;

    QuadTreeNode* rootNode() const;
    void reset();
    QRectF bounds() const;
    ~QuadTree();
private:
    bool init(const QRectF& bounds);
    bool _addPoint(const Point* m);
    bool initLite(const QRectF& bounds);
    bool initNodesPos(const QRectF& bounds);
    QRectF computeMinRect();
    QuadTreeParams mParams;
    QuadTreeNode* mNodes;
    QuadTreeNode* mLevels[8];
    QVector2D mPos;
    qreal mRadius;
    qreal mInverseRadius;
    std::set<const Point*> mPoints;
};

inline QRectF unionRectVec(const QRectF& rect, const QVector2D& pos) {
    qreal left = std::min((float)rect.left(),pos.x());
    qreal top = std::min((float)rect.top(),pos.y());
    qreal right = std::max((float)rect.right(),pos.x());
    qreal bottom = std::max((float)rect.bottom(),pos.y());
    return QRectF(
                left,
                top,
                right-left,
                bottom-top
                );
}


#endif // INSTANTQUADTREE_H
