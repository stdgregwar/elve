#include "LayoutPolicy.h"
#include <QDebug>
#include <random>
#include <chrono>
#include <QGraphicsScene>

using namespace std;

LayoutPolicy::LayoutPolicy(QGraphicsScene& scene) : mScene(scene)
{

}

void LayoutPolicy::clear() {
    mEdges.clear();
    mNodes.clear();
    mSystem.clear();
    mScene.clear();
}

NodePositions LayoutPolicy::nodesPositions() const {
    NodePositions positions;
    for(auto p : mMasses) {
        positions[p.first] = p.second->pos();
    }
    return positions;
}

void LayoutPolicy::setGraph(SharedGraph graph) {
    size_t simticks = 800;
    default_random_engine gen;
    std::uniform_int_distribution<int> u(-1024,1024);
    NodePositions randomPos;
    for(const auto& p : graph->gates()) {
        float x = u(gen);
        float y = u(gen);
        randomPos.emplace(p.second.id(),QVector2D{x,y});
    }

    setGraph(graph,randomPos);

    quickSim(simticks);
    //mScene.setSceneRect(mScene.itemsBoundingRect());
}

const MassesByID& LayoutPolicy::massesByID() const {
    return mMasses;
}

const System& LayoutPolicy::system() const
{
    return mSystem;
}

void LayoutPolicy::quickSim(size_t ticks) {
    if(ticks == 0)
        return;
    qDebug() << "Simulating with" << mSystem.massCount() << "masses and" << mSystem.forceCount() << "forces!";
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < ticks; i++) {
        tick(0.1,false);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    qDebug() << "Simulating at mean of" << ticks/time_span.count() << "tps";
}

EdgeItem* LayoutPolicy::newEdge(size_t segments)
{
    EdgeItem* ne = new EdgeItem(segments);
    mScene.addItem(ne);
    mEdges.push_back(ne);
    return ne;
}

NodeItem* LayoutPolicy::newNode(const NodeID& id, const Node::Type &t)
{
    NodeItem* nn = new NodeItem(id,t);
    mScene.addItem(nn);
    mNodes.push_back(nn);
    return nn;
}

void LayoutPolicy::tick(float dt, bool update)
{
    mSystem.tick(dt,update);
}
