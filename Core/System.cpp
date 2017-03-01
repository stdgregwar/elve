#include "System.h"
#include "Damp.h"
#include "LinearConstraint.h"
#include "PointConstraint.h"

#include <limits>
#include <future>
#include <QDebug>

namespace Elve {

using namespace std;

System::System() : mGravity(75e4)
{

}

void System::tick(float dt, bool update)
{
    static int count = 0;
    //qDebug() << "tick" << count++;
    mGravity.updateQuadTree();

    int nthreads = 8;
    vector<future<void>> futures(nthreads);
    int splitsize = mPoints.size() / nthreads;
    size_t end = 0;

    for(int i = 0; i < nthreads-1; i++) {
        end = (i+1)*splitsize;
        futures[i] = async(bind(&System::computeForces,this,i*splitsize,end));
    }
    futures[nthreads-1] = async(bind(&System::computeForces,this,end,mPoints.size()));

    for(future<void>& f : futures) {
        f.get();
    }

    for(Point* m : mPoints) {
        m->tick(dt,update);
    }
}

void System::debug(QPainter* p) const {
    mGravity.debug(p);
}

void System::computeForces(size_t from,size_t until)
{
    for(size_t i = from; i < until; i++) {
        mPoints[i]->resetForce();
        mPoints[i]->computeForce();
    }
}

const QRectF& System::sizeHint() const {
    return mSizeHint;
}

Point* System::point(const NodeID& id) {
    auto it = mPointsById.find(id);
    if(it != mPointsById.end()) {
        return it->second;
    } else {
        return addPoint(1,id,{0,0},3,FULL); //Fallback point, should never happen
    }
}

const PointsByID& System::pointsByID() const {
    return mPointsById;
}

NodePositions System::positions() const
{
    NodePositions poss;
    for(const Point* p : mPoints) {
        poss[p->boundID()] = p->pos();
    }
    return poss;
}

void System::clearMovables() {
    for(Point* p : mPoints) {
        p->clearMovables();
    }
}

Point* System::addPoint(qreal mass, const NodeID &id, QVector2D pos, qreal damp, GravityMode g)
{
    Point* nm = new Point(mass,id);
    mPoints.push_back(nm);
    mPointsById[id] = nm;
    nm->setPos(pos);
    //id->setMass(nm);
    if(g != NONE) {
        nm->addForce(&mGravity);
        if(g == FULL) {
            mGravity.addPoint(nm);
        }
    }
    Damp* d = new Damp(damp);
    nm->addForce(d);
    nm->addConstraint(&mBox);
    return nm;
}

void System::addSpring(unsigned i, unsigned j, qreal k, qreal l0)
{
    Point* mi = mPoints.at(i);
    Point* mj = mPoints.at(j);
    addSpring(mi,mj,k,l0);
}

void System::addSpring(Point* mi, Point* mj, qreal k, qreal l0)
{
    Spring* force = new Spring(*mi,*mj,l0,k);
    mi->addForce(force);
    mj->addForce(force);
    mForces.push_back(force);
}
void System::clear()
{
    for(Point* m : mPoints) {
        delete m;
    }

    mPoints.clear();
    for(Force* f : mForces) {
        delete f;
    }
    mForces.clear();
    mGravity.clear();
}

const Point* System::nearest(const QVector2D& p) const
{
    qreal dist = std::numeric_limits<qreal>::infinity();
    const Point* ml = mPoints.back();
    for(const Point* m : mPoints) {
        qreal r = (m->pos() - p).lengthSquared();
        if(r < dist) {
            dist = r;
            ml = m;
        }
    }
    return ml;
}

void System::setSizeHint(const QRectF& rect) {
    mGSizeHint = rect.adjusted(-512,-512,512,512);
    switch(orientationHint()) {
    case LEFTRIGHT:
    case RIGHTLEFT:
        mSizeHint = QRectF(mGSizeHint.top(),mGSizeHint.left(),mGSizeHint.height(),mGSizeHint.width());
        break;
    case TOPDOWN:
    default:
        mSizeHint = mGSizeHint;
        break;
    }

    mGravity.setQuadTreeBounds(mSizeHint);
    mBox.setBounds(mSizeHint);
}

LinearConstraint::Dir _adaptDir(OrientationHint hint, LinearConstraint::Dir dir) {
    switch(hint) {
    case RIGHTLEFT:
    case LEFTRIGHT:
        return (LinearConstraint::Dir)((dir+1)%2); //TODO explicit this
    case TOPDOWN:
    default:
       return dir;
    }
}

qreal _adaptPos(QVector2D transformPoint,LinearConstraint::Dir dir) {
    return dir == LinearConstraint::VERTICAL ? transformPoint.y() : transformPoint.x();
}

void System::addVConstraint(Point* m, qreal height)
{
    qreal hardn = 0.6;
    LinearConstraint::Dir newDir = _adaptDir(orientationHint(),LinearConstraint::VERTICAL);
    qreal tpos = _adaptPos(transformPoint({0,height}),newDir);
    Constraint* c = new LinearConstraint(tpos,hardn,newDir);
    m->addConstraint(c);
    mConstraints.push_back(c);
}

void System::addHConstraint(Point* m, qreal pos) {
    qreal hardn = 0.6;
    Constraint* c = new LinearConstraint(transformPoint({pos,0}).x(),hardn,
                                         _adaptDir(orientationHint(),LinearConstraint::HORIZONTAL));
    m->addConstraint(c);
    mConstraints.push_back(c);
}

QVector2D System::transformPoint(const QVector2D& p) const {
    switch(orientationHint()) {
    case TOPDOWN:
        return {p.x(), mGSizeHint.bottom() - (p.y() - mGSizeHint.top())};
    case LEFTRIGHT:
        return {mGSizeHint.bottom() - (p.y() - mGSizeHint.top()),p.x()};
    case RIGHTLEFT:
        return {p.y(),p.x()};
    default:
       return p;
    }
}

void System::addPConstrain(Point* m, const QVector2D& p)
{
    Constraint* pc = new PointConstraint(transformPoint(p));
    m->addConstraint(pc);
    mConstraints.push_back(pc);
}

void System::setOrientationHint(OrientationHint hint) {
    mOrHint = hint;
}

OrientationHint System::orientationHint() const {
    return mOrHint;
}

size_t System::massCount() const
{
    return mPoints.size();
}

size_t System::forceCount() const
{
    return mForces.size();
}

System::~System()
{
    clear();
}

}
