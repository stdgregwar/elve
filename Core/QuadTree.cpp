#include "QuadTree.h"
#include <cmath>
#include <assert.h>
#include <algorithm>
#include <QDebug>
#include <limits>

using namespace std;
#define THRESHOLD 1024

QuadTree::QuadTree(const QRectF &bounds) : mParams{THRESHOLD*THRESHOLD,10}
{
    mNodes = nullptr;
    init(bounds);
}

QuadTree& QuadTree::operator=(QuadTree&& other) {
    mNodes = other.mNodes;
    other.mNodes = nullptr;
    init(other.bounds());
}

bool QuadTree::init(const QRectF &bounds)
{
    reset();

    if(!mNodes) {
        mNodes = reinterpret_cast<QuadTreeNode*>(new unsigned char[QUADTREESIZE*sizeof(QuadTreeNode)]);
    }
    if(!mNodes) {return false;}
    QuadTreeNode* root = rootNode();

    for(long i = QUADTREESIZE;i--;)
    {
        new (&root[i])QuadTreeNode();
    }
    return initLite(bounds);
}

bool QuadTree::initLite(const QRectF& bounds) {
    // Nodes are stored so that all nodes in a level are sequential in memory until
    //	the next level.  Within each level, the nodes are 2D arrays of equal width
    //	and height, double the previous layer.  There are 8 layers, with the first
    //	being 1-by-1 and the last being 128-by-128.
    //
    // This loop goes over each level

    mRadius = max(bounds.width()*0.5f,bounds.height()*0.5f);

    mInverseRadius = 255.f / (2*mRadius);
    mPos = {bounds.left() + bounds.width()*0.5f, bounds.top() + bounds.height()*0.5f};;

    QuadTreeNode* root = rootNode();
    float radius = mRadius;
    QuadTreeNode* pqtnThis = root, *pqtnLast = nullptr;
    float fFull = radius*2.f;

    QVector2D vOffset( -radius, -radius);
    for (unsigned I = 0UL; I < 8UL; ++I ) {
        mLevels[I] = pqtnThis;
        // For each node at this level.
        for (unsigned Y = (1UL << I); Y--; ) {
            QuadTreeNode* pqtnRow = &pqtnThis[Y*(1UL<<I)];
            float fYPos = float( Y ) / float( 1UL << I ) * fFull + radius + mPos.y();
            for ( unsigned X = (1UL << I); X--; ) {
                pqtnRow[X].setCenterAndRadius(QVector2D(float( X ) / float( 1UL << I ) * fFull + radius + mPos.x(), fYPos ) + vOffset,
                    radius);

                if ( pqtnLast ) {
                    // There is a parent.  Get its index.
                    unsigned ui32ParentX = X >> 1UL;
                    unsigned ui32ParentY = Y >> 1UL;
                    unsigned ui32ParentIndex = (ui32ParentY * (1UL << (I - 1UL))) + ui32ParentX;
                    unsigned ui32X = X & 1UL;
                    unsigned ui32Y = Y & 1UL;
                    pqtnLast[ui32ParentIndex].setChild((ui32Y << 1UL) + ui32X, &pqtnRow[X] );
                }
            }
        }

        pqtnLast = pqtnThis;
        pqtnThis += (1UL << I) * (1UL << I);
        radius *= 0.5f;
    }

    return true;
}

bool QuadTree::initNodesPos(const QRectF& bounds)
{
    // Nodes are stored so that all nodes in a level are sequential in memory until
    //	the next level.  Within each level, the nodes are 2D arrays of equal width
    //	and height, double the previous layer.  There are 8 layers, with the first
    //	being 1-by-1 and the last being 128-by-128.
    //
    // This loop goes over each level

    mRadius = max(bounds.width()*0.5f,bounds.height()*0.5f);
    mInverseRadius = 255.f / (2*mRadius);
    mPos = {bounds.left() + bounds.width()*0.5f, bounds.top() + bounds.height()*0.5f};;

    QuadTreeNode* root = rootNode();
    float radius = mRadius;
    QuadTreeNode* pqtnThis = root, *pqtnLast = nullptr;
    float fFull = radius*2.f;

    QVector2D vOffset( -radius, -radius);
    for (unsigned I = 0UL; I < 8UL; ++I ) {
        mLevels[I] = pqtnThis;
        // For each node at this level.
        for (unsigned Y = (1UL << I); Y--; ) {
            QuadTreeNode* pqtnRow = &pqtnThis[Y*(1UL<<I)];
            float fYPos = float( Y ) / float( 1UL << I ) * fFull + radius + mPos.y();
            for ( unsigned X = (1UL << I); X--; ) {
                pqtnRow[X].setCenterAndRadius(QVector2D(float( X ) / float( 1UL << I ) * fFull + radius + mPos.x(), fYPos ) + vOffset,
                    radius);
            }
        }
        pqtnLast = pqtnThis;
        pqtnThis += (1UL << I) * (1UL << I);
        radius *= 0.5f;
    }
    return true;
}

QRectF QuadTree::bounds() const {
    return QRectF{
              mPos.x() - mRadius,
              mPos.y() - mRadius,
              mRadius*2,
              mRadius*2
    };
}

bool QuadTree::addPoint(const Point *m)
{
    mPoints.insert(m);
    return _addPoint(m);
}

bool QuadTree::_addPoint(const Point *m)
{
    //qDebug() << "bounds" << bounds();
    if(bounds().contains(m->pos().toPointF())) {
        return rootNode()->addPoint(m,mParams);
    } else { //Trigger resizing of the quadtree
        qDebug() << "resizing quad tree!!! because of" << m->pos();
        QRectF newRect = unionRectVec(bounds(),m->pos());
        newRect.adjust(-100,-100,100,100);
        initNodesPos(newRect);
        for(const Point* r : mPoints) { //Re add all objects
            _addPoint(r);
        }
        //addObject(obj); //Finaly add obj
    }
    return false;
}

QRectF QuadTree::computeMinRect()
{
    QRectF r(0,0,0,0);
    //qDebug() << "mass count : " << mMasses.size();
    for(const Point* m : mPoints) {
        r = unionRectVec(r,m->pos());
    }
    return r.adjusted(-100,-100,100,100);
}

void QuadTree::reinsertAll()
{
    static int count = 0;
    count++;
    for(long i = QUADTREESIZE; i--;) {
        mNodes[i].reset();
    }

    if(count) {
        QRectF rect = computeMinRect();
        initNodesPos(computeMinRect());
        count = 0;
    }

    for(const Point* m : mPoints) {
        _addPoint(m);
    }
}

void QuadTree::removePoint(const Point *m)
{
    QuadTreeNode* node = static_cast<QuadTreeNode*>(m->containerData());
    if(node) {
        node->remPoint(m,mParams);
    }
}

void QuadTree::clear()
{
    for(long i = QUADTREESIZE; i--;) {
        mNodes[i].reset();
    }
    mPoints.clear();
}

void QuadTree::movePoint(const Point *m)
{
    QuadTreeNode* node = static_cast<QuadTreeNode*>(m->containerData());
    if(node && !node->contain(m)) {
        removePoint(m);
        addPoint(m);
    } else {
        addPoint(m);
    }
}

QuadTreeNode* QuadTree::rootNode() const
{
    return mNodes;
}


QVector2D QuadTree::gravityFor(const Point& m) const
{
    return rootNode()->gravityFor(m,mParams);
}

void QuadTree::reset()
{
    if(mNodes) {
        // The number of nodes is fixed.
        for (long i = QUADTREESIZE; i--;) {
            mNodes[i].~QuadTreeNode();
        }
        //free((void*)mNodes);
        //mNodes = nullptr;
    }
}

QuadTree::~QuadTree()
{
    reset();
    if(mNodes)
        free((void*)mNodes);
}
