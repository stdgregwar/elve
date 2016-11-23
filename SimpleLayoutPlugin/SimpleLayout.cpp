#include "SimpleLayout.h"
#include <cmath>
#include <QDebug>

void SimpleLayout::setGraph(SharedGraph graph)
{
    clear();
    qreal sk = 2;
    qreal l0 = 0;
    qreal damp = 2;
    qreal unit = std::max(qreal(graph->nodeCount())/20,64.0);

    qreal totHeight = graph->highestLevel()*unit;
    qreal inputHeight = totHeight/2;
    qreal outputHeight = -totHeight/2;

    qreal ioFactor = (qreal)(graph->outputCount()) / graph->inputCount();
    qreal ioUnit = std::max(totHeight/graph->outputCount(),128.0);

    qDebug() << ioUnit << ioFactor;

    for(const auto& p : graph->nodes()) {
        QVector2D pos = startPosition(p.first);

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

        for(const Node* n : g.children()) {
            Point* me = system().point(n->id());
            system().addSpring(ms,me,sk/congr,l0*congr);
        }
    }
}

void SimpleLayout::tick(float dt, bool fast)
{
    LayoutPlugin::tick(dt,fast);
}
