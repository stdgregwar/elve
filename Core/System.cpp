#include "System.h"
#include "Damp.h"
#include "VerticalConstraint.h"
#include "PointConstraint.h"

#include <limits>
#include <future>

using namespace std;

System::System() : mGravity(7e4)
{

}

void System::tick(float dt, bool update)
{
    /*for(Mass* m : mMasses) {
        m->resetForce();
        m->computeForce();
    }*/

    int nthreads = 8;
    vector<future<void>> futures(nthreads);
    int splitsize = mMasses.size() / nthreads;
    size_t end = 0;

    for(int i = 0; i < nthreads-1; i++) {
        end = (i+1)*splitsize;
        futures[i] = async(bind(&System::computeForces,this,i*splitsize,end));
    }
    futures[nthreads-1] = async(bind(&System::computeForces,this,end,mMasses.size()));

    for(future<void>& f : futures) {
        f.get();
    }

    for(Point* m : mMasses) {
        m->tick(dt,update);
    }
}

void System::computeForces(size_t from,size_t until)
{
    for(size_t i = from; i < until; i++) {
        mMasses[i]->resetForce();
        mMasses[i]->computeForce();
    }
}

Point* System::addPoint(qreal mass, Movable* m, QVector2D pos, qreal damp, GravityMode g)
{
    Point* nm = new Point(mass,m);
    mMasses.push_back(nm);
    nm->setPos(pos);
    m->setMass(nm);
    if(g != NONE) {
        nm->addForce(&mGravity);
        if(g == FULL) {
            mGravity.addMass(nm);
        }
    }
    Damp* d = new Damp(damp);
    nm->addForce(d);
    return nm;
}

void System::addSpring(unsigned i, unsigned j, qreal k, qreal l0)
{
    Point* mi = mMasses.at(i);
    Point* mj = mMasses.at(j);
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
    for(Point* m : mMasses) {
        delete m;
    }

    mMasses.clear();
    for(Force* f : mForces) {
        delete f;
    }
    mForces.clear();
    mGravity = Gravity(7e4);
}

const Point* System::nearest(const QVector2D& p) const
{
    qreal dist = std::numeric_limits<qreal>::infinity();
    const Point* ml = mMasses.back();
    for(const Point* m : mMasses) {
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
    return mMasses.size();
}

size_t System::forceCount() const
{
    return mForces.size();
}

System::~System()
{
    clear();
}
