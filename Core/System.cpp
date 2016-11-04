#include "System.h"
#include "Damp.h"
#include "VerticalConstraint.h"
#include "PointConstraint.h"

#include <limits>
#include <future>
#include <QDebug>

using namespace std;

System::System() : mGravity(7e4)
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

void System::addVConstraint(Point* m, qreal height)
{
    Constraint* vc = new VerticalConstraint(height);
    m->addConstraint(vc);
    mConstraints.push_back(vc);
}

void System::addPConstrain(Point* m, const QVector2D& p)
{
    Constraint* pc = new PointConstraint(p);
    m->addConstraint(pc);
    mConstraints.push_back(pc);
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
