#include "SimpleLayout.h"
#include <cmath>
#include <QDebug>

SimpleLayout::SimpleLayout() {
    opts().add_options()
            ("k_const,k",po::value(&mK)->default_value(mK),"base k constant of the springs")
            ("l_zero,l",po::value(&mL0)->default_value(mL0),"length of the springs at rest")
            ("damp,d",po::value(&mDamp)->default_value(mDamp),"damping factor of the points")
            ("unitLength,u",po::value(&mMinUnit)->default_value(mMinUnit),"minimal level unit length in pixels")
            ("iosUnitLength,o",po::value(&mMinIOUnit)->default_value(mMinIOUnit), "minimal space between two I/Os")
            ;
}

void SimpleLayout::setGraph(SharedGraph graph)
{
    clear();
    qreal sk = mK;
    qreal l0 = mL0;
    qreal damp = mDamp;
    qreal unit = std::max(qreal(graph->nodeCount())/20,mMinUnit);

    qreal totHeight = graph->highestLevel()*unit;
    qreal inputHeight = totHeight/2;
    qreal outputHeight = -totHeight/2;

    qreal maxOut = graph->maxOutputIndex();
    qreal maxIn = graph->maxInputIndex();
    qreal ioFactor = (qreal)(graph->maxOutputIndex()) / graph->maxInputIndex();
    qreal ioUnit = std::max(totHeight/graph->maxOutputIndex(),mMinIOUnit);

    QRectF rect(0,outputHeight,graph->maxOutputIndex()*ioUnit,totHeight);
    system().setSizeHint(rect);

    for(const auto& p : graph->nodes()) {
        QVector2D pos = startPosition(p.first,rect);
        Point* m = system().addPoint(1,p.second.id(),pos,damp,FULL);

        if(p.second.isInput()) {
            //mSystem.addVConstraint(m,1024*SS);

            //Interleaving inputs
            int index = p.second.IOIndex();
            int index1 = index*2;
            int index2 = (index-(graph->inputCount()/2))*2+1;
            int t_index = index;// > (graph->inputCount()/2) ? index2 : index1;
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
