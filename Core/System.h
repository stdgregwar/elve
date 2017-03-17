#ifndef SYSTEM_H
#define SYSTEM_H

#include "Point.h"
#include "Spring.h"
#include "Force.h"
#include "Gravity.h"
#include "Constraint.h"
#include "Graph.h"
#include "BoxConstraint.h"
#include "interfaces/NodeLook.h"

#include <vector>
#include <unordered_map>

namespace Elve {

enum GravityMode {
    FULL,
    GHOST,
    NONE
};

typedef std::unordered_map<NodeID,QVector2D> NodePositions;
typedef std::unordered_map<NodeID,Point*> PointsByID;

class System
{
public:
    System();
    void tick(float dt,bool update = true);
    Point* addPoint(qreal mass, const NodeID& id, QVector2D pos, qreal damp, GravityMode g = FULL);
    Point* point(const NodeID& id);
    void addSpring(unsigned i, unsigned j, qreal k, qreal l0);
    void addSpring(Point* mi, Point* mj, qreal k, qreal l0);
    void addVConstraint(Point* m, qreal height);
    void addHConstraint(Point* m, qreal pos);
    void addPConstrain(Point* m, const QVector2D& p);
    void clear();
    void clearMovables();
    size_t massCount() const;
    size_t forceCount() const;
    const Point* nearest(const QVector2D& p) const;
    NodePositions positions() const;
    const PointsByID& pointsByID() const;
    void debug(QPainter* p) const;
    void setSizeHint(const QRectF& rect);
    void setOrientationHint(OrientationHint hint);
    void setRepulsionForce(float f);
    OrientationHint orientationHint() const;
    QVector2D transformPoint(const QVector2D& p) const;
    const QRectF& sizeHint() const;
    ~System();
private:
    QRectF mSizeHint;
    QRectF mGSizeHint;
    OrientationHint mOrHint;
    void computeForces(size_t from,size_t until);
    Gravity mGravity;
    BoxConstraint mBox;
    std::vector<Point*> mPoints;
    PointsByID mPointsById;
    std::vector<Force*> mForces;
    std::vector<Constraint*> mConstraints;
};

}

#endif // SYSTEM_H
