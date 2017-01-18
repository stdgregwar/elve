#include "QuadTreeNode.h"
#include <QDebug>

#include <QPen>
#include <QPainter>

using namespace std;

//#define DEBUG_QUAD

QuadTreeNode::QuadTreeNode()
{
    reset();
    mPoints.reserve(100); //Should be enough
    for(int i = 0; i < 4; i++)
        mChildren[i] = nullptr;
    //pen().setColor(Qt::red);
}

void QuadTreeNode::setCenterAndRadius(const QVector2D& pos, float radius)
{
    mCenter = pos;
    mRadius = radius;
}

void QuadTreeNode::setChild(unsigned index, QuadTreeNode* node)
{
    node->setParent(this);
    mChildren[index] = node;
}

void QuadTreeNode::debug(QPainter* p) const
{
    if(leaf() && mPoints.size()) {
        p->drawRect(mCenter.x()-mRadius,mCenter.y()-mRadius,2*mRadius,2*mRadius);
        QPointF c = CoM().toPointF();
        //p->drawEllipse(c,4096,4096);
        p->drawEllipse(c,32,32);
        p->drawText(c,QString::number(mMass));
    }
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
    qreal l = r.length();

    qreal ratio = (mRadius*2) / l;
    if(ratio < params.theta) {
        /*if(leaf()) {
            return r.normalized() * ((mMass*m.mass()) / lr);
        } else {
            for(int i = 0; i < 4; i++) {
                f += mChildren[i]->gravityFor(m,params);
            }
        }*/
        qreal lr = l*l;
        return -(r / l) * ((mMass*m.mass()) / lr);
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
    if(mMass) {
        return mMC / mMass;
    } else {
        return mCenter;
    }

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

