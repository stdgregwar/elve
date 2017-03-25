#include "LayoutPlugin.h"
#include <random>
#include <chrono>
#include <QDebug>
#include <Graph.h>
#include <ExtendedGraph.h>
#include "GraphWidgetListener.h"

namespace Elve {
using namespace std;

LayoutPlugin::LayoutPlugin()
{

}

LayoutPlugin::LayoutPlugin(const LayoutPlugin& other) :
    QObject(other.parent())
{

}

QVector2D LayoutPlugin::startPosition(const NodeID& id) {
    static default_random_engine gen;
    const QRectF& rect = mSystem.sizeHint();

    auto it = mStartPositions.find(id);
    if(it != mStartPositions.end()) {
        return it->second;
    } else {
        std::uniform_real_distribution<qreal> x(rect.left(),rect.right());
        std::uniform_real_distribution<qreal> y(rect.top(),rect.bottom());
        QVector2D p(x(gen),y(gen));
        mStartPositions[id] = p;
        return p;
    }
}

void LayoutPlugin::setGraph(SharedEGraph g,const NodePositions& positions)
{
    mSystem.setOrientationHint(g->look()->orientationHint());
    mStartPositions = positions;
    setGraph(g->graph());
}

void LayoutPlugin::clear(){
    mSystem.clear();
}

System& LayoutPlugin::system() {
    return mSystem;
}

QJsonObject LayoutPlugin::json() const {
    QJsonObject layout;
    QJsonObject  positions;

    layout.insert("name",name());

    using pair_type = NodePositions::value_type;
    for(const pair_type& p : system().positions()) {
        const QVector2D& pos = p.second;
        positions.insert(QString::number(p.first),
                         QJsonArray{pos.x(),pos.y()});
    }
    layout.insert("positions",positions);
    QJsonArray pinned;
    for(const PointsByID::value_type& p : system().pinnedPoints()) {
        pinned.append((int)p.first);
    }
    layout.insert("pinned",pinned);
    return layout;
}

void LayoutPlugin::fromJson(const QJsonObject& layout) {
    QJsonArray pinned = layout.value("pinned").toArray();
    QJsonObject poss = layout.value("positions").toObject();
    for(const QJsonValue& v : pinned) {
        QJsonArray parr = poss.value(QString::number(v.toInt())).toArray();
        QVector2D pos(parr[0].toDouble(),parr[1].toDouble());
        system().pin(v.toInt(),pos);
    }
}

const System& LayoutPlugin::system() const
{
    return mSystem;
}

void LayoutPlugin::tick(float dt, bool fast) {
    mSystem.tick(dt,fast);
}

void LayoutPlugin::quickSim(size_t ticks) {
    if(ticks == 0)
        return;
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < ticks; i++) {
        tick(0.25,false);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    qDebug() << "Simulating at mean of" << ticks/time_span.count() << "tps";
}

void LayoutPlugin::uiStart(Elve::GraphWidgetListener* listener,const SharedEGraph& graph) {
    listener->runCommand(QString("%1_layout").arg(cliName().c_str()));
}

}
