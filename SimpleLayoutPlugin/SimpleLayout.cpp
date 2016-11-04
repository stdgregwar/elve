#include "SimpleLayout.h"
#include <cmath>
#include <QDebug>

SimpleLayout::SimpleLayout(QObject *parent) :
    QObject(parent)
{
}

void SimpleLayout::setGraph(SharedGraph graph,const NodePositions& positions)
{
    clear();
    qreal sk = 8;
    qreal l0 = 0;
    qreal damp = 2;
    qreal unit = 64;

    qreal totHeight = graph->highestLevel()*unit;
    qreal inputHeight = totHeight/2;
    qreal outputHeight = -totHeight/2;

    qreal ioFactor = (qreal)(graph->outputCount()) / graph->inputCount();
    qreal ioUnit = totHeight/graph->outputCount();

    qDebug() << ioUnit << ioFactor;

    for(const auto& p : graph->nodes()) {
        const QVector2D& pos = positions.at(p.second.id());

        Point* m = system().addPoint(1,p.second.id(),pos,damp,FULL);

        if(p.second.isInput()) {
            //mSystem.addVConstraint(m,1024*SS);

            //Interleaving inputs
            int index = p.second.IOIndex();
            int index1 = index*2;
            int index2 = (index-(graph->inputCount()/2))*2+1;
            int t_index = index > (graph->inputCount()/2) ? index2 : index1;
            system().addPConstrain(m,{t_index*ioUnit*ioFactor,inputHeight});
        } else if(p.second.isOutput()) {
            //mSystem.addVConstraint(m,-1024*SS);
            system().addPConstrain(m,{p.second.IOIndex()*ioUnit,outputHeight});
        } else {
            //system().addVConstraint(m,1024-64*p.second.level());
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

void SimpleLayout::tick(float dt, bool fast)
{
    LayoutPlugin::tick(dt,fast);
}

QString SimpleLayout::layoutName()
{
    return "Simple-Force";
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(LevelLayoutPlugin, LevelLayoutPlugin)
#endif // QT_VERSION < 0x050000
