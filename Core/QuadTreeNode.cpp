#include "QuadTreeNode.h"
#include <QDebug>

#include <QPen>

using namespace std;

//#define DEBUG_QUAD

QuadTreeNode::QuadTreeNode()
{
    reset();
    mPoints.reserve(20); //Should be enough
    //pen().setColor(Qt::red);
}

void QuadTreeNode::setCenterAndRadius(const QVector2D& pos, float radius)
{
    mCenter = pos;
    mRadius = radius;

#ifdef DEBUG_QUAD
    setRect(mCenter.x()-mRadius, //Visible rect
        mCenter.y()-mRadius,
        2*mRadius, 2*mRadius);
#endif

}

void QuadTreeNode::setChild(unsigned index, QuadTreeNode* node)
{
    node->setParent(this);
    mChildren[index] = node;
}

bool QuadTreeNode::addPoint(const Point *m, const QuadTreeParams &params)
{
    if(contain(m)) {
        //qDebug() << mCenter << mRadius << "Contains!" << m->pos();
        mCount++;
        //qDebug() << "count" << mCount << "masses" << mMasses.size();
        if(mCount < params.maxMasses || !mChildren[0]) {
            //qDebug() << "inserting it";
            mPoints.push_back(m);
            m->setContainerData(this);
        } else {
            //qDebug() << "no more space splitting";
            for(const Point* om : mPoints) {
                for(int i = 0; i < 4; i++) {
                    mChildren[i]->addPoint(om,params);
                }
            }
            mPoints.clear();
            for(int i = 0; i < 4; i++) { //Add new mass
                mChildren[i]->addPoint(m,params);
            }
        }
        mMC += m->pos()*m->mass();
        mMass += m->mass();
#ifdef DEBUG_QUAD
        mGMC->setPos(CoM().toPointF());
#endif
        return true;
    } else {
        return false;
    }
}

std::vector<const Point *> &QuadTreeNode::givePoints()
{
    mCount = 0;
    mMass = 0;
    mMC = {0,0};
    return mPoints;
}

void QuadTreeNode::remPoint(const Point *m, const QuadTreeParams &params)
{
    mCount--;
    mPoints.erase(remove(mPoints.begin(),mPoints.end(),m),mPoints.end());
    mMass -= m->mass();
    mMC -= m->pos();

    if(mCount <= params.maxMasses) { //call masses one level up
        for(int i = 0; i < 4 && mChildren[i]; i++) {
            vector<const Point*>& mset = mChildren[i]->givePoints();
            for(const Point* ms : mset) {
                ms->setContainerData(this);
            }
            mPoints.insert(mPoints.end(),mset.begin(),mset.end());
            mset.clear();
            //qDebug() << "hiding" << mChildren[i];
        }
    }
    if(mParent) {
        mParent->remPoint(m,params);
    }
}

void QuadTreeNode::setParent(QuadTreeNode *parent)
{
    mParent = parent;
}

void QuadTreeNode::reset()
{
    mCount = 0;
    mMass = 0;
    mMC = {0,0};
    mParent = nullptr;
    mPoints.clear();
    /*for(int i = 0; i < 4; i++)
        mChildren[i] = nullptr;*/
}

const QVector2D& QuadTreeNode::getCenter() const
{
    return mCenter;
}

const qreal& QuadTreeNode::getRadius() const
{
    return mRadius;
}

const qreal& QuadTreeNode::mass() const
{
    return mMass;
}

QVector2D QuadTreeNode::gravityFor(const Point& m, const QuadTreeParams& params) const
{
    QVector2D f;
    QVector2D r = (CoM() - m.pos());
    qreal lr = r.lengthSquared();
    if(lr > params.gravDistSquare && mMass > 1e-7) {
        //qDebug() << "Approx for " << m.pos();
        return r.normalized() * ((mMass*m.mass()) / lr);
    } else {
        if(leaf()) {
            return trueGravity(m);
        } else {
            for(int i = 0; i < 4; i++) {
                f += mChildren[i]->gravityFor(m,params);
            }
        }
    }
    return f;
}

QVector2D QuadTreeNode::CoM() const {
    return mMC / mMass;
}

QVector2D QuadTreeNode::trueGravity(const Point &m) const {
    QVector2D f{0,0};
    for(const Point* const& ms : mPoints) {
        if(ms != &m) {
            QVector2D r = ms->pos() - m.pos();
            qreal lr = r.lengthSquared();
            if(lr > 1e-2) {
                f += -r.normalized() * ((ms->mass()*m.mass()) / lr);
            }
        }
    }
    return f;
}

bool QuadTreeNode::contain(const Point *o) const
{
    const QVector2D& pos = o->pos();
    return (
                mCenter.x() - mRadius <= pos.x() &&
                mCenter.y() - mRadius <= pos.y() &&
                mCenter.x() + mRadius > pos.x() &&
                mCenter.y() + mRadius > pos.y()
                );
}

bool QuadTreeNode::leaf() const {
    return mCount == mPoints.size() || !mChildren[0];
}

QuadTreeNode::~QuadTreeNode()
{
    reset();
}

