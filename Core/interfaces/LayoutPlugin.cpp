#include "LayoutPlugin.h"
#include <random>
#include <chrono>
#include <QDebug>
#include <Graph.h>

using namespace std;

LayoutPlugin::LayoutPlugin()
{

}

LayoutPlugin::LayoutPlugin(const LayoutPlugin& other) :
    QObject(other.parent())
{

}

QVector2D LayoutPlugin::startPosition(const NodeID& id,QRectF rect) {
    static default_random_engine gen;

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

void LayoutPlugin::setGraph(SharedGraph g,const NodePositions& positions)
{
    mStartPositions = positions;
    setGraph(g);
}

void LayoutPlugin::clear(){
    mSystem.clear();
}

System& LayoutPlugin::system() {
    return mSystem;
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
        tick(0.4,false);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    qDebug() << "Simulating at mean of" << ticks/time_span.count() << "tps";
}
