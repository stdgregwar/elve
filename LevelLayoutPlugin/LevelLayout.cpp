#include "LevelLayout.h"
#include <cmath>

LevelLayout::LevelLayout(QObject *parent) :
    QObject(parent)
{
}

void LevelLayout::setGraph(SharedGraph graph,const NodePositions& positions)
{
    clear();
    qreal sk = 8;
    qreal l0 = 0;
    qreal damp = 2;

    for(const auto& p : graph->nodes()) {
        const QVector2D& pos = positions.at(p.second.id());

        Point* m = system().addPoint(1,p.second.id(),pos,damp,FULL);

        if(p.second.isInput()) {
            //mSystem.addVConstraint(m,1024*SS);
            system().addPConstrain(m,{p.second.IOIndex()*192,1024});
        } else if(p.second.isOutput()) {
            //mSystem.addVConstraint(m,-1024*SS);
            system().addPConstrain(m,{p.second.IOIndex()*192,-1024});
        } else {
            system().addVConstraint(m,1024-64*p.second.level());
        }
    }

    for(const auto& p : graph->nodes()) {
        const Node& g =  p.second;
        Point* ms = system().point(g.id());

        float congr = std::pow(g.children().size(),2);

        for(const Node::Child& c : g.children()) {
            Point* me = system().point(c.node->id());
            system().addSpring(ms,me,sk/congr,l0*congr);
        }
    }
}

void LevelLayout::tick(float dt, bool fast)
{
    LayoutPlugin::tick(dt,fast);
}

QString LevelLayout::layoutName()
{
    return "Level-Force";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(LevelLayoutPlugin, LevelLayoutPlugin)
#endif // QT_VERSION < 0x050000
