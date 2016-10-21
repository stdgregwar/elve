#include "LevelForceLayout.h"
#include <cmath>

#include <QDebug>

LevelForceLayout::LevelForceLayout(QGraphicsScene& scene) : LayoutPolicy(scene)
{

}

void LevelForceLayout::setGraph(SharedGraph graph,const NodePositions& positions)
{
    qDebug() << "Displaying a" << graph->gateCount() << "entities graph!";

    qreal sk = 8;
    qreal l0 = 0;
    qreal damp = 2;
    clear();

    for(const auto& p : graph->gates()) {
        NodeItem* n = newNode(p.second.id(),p.second.type());
        const QVector2D& pos = positions.at(p.second.id());

        //qDebug() << "x,y" << x << y;
        Mass* m = mSystem.addMass(1,n,pos,damp,FULL);

        if(p.second.isInput()) {
            //qDebug() << p.second.id().c_str() << "is input";
            //mSystem.addVConstraint(m,1024*SS);
            mSystem.addPConstrain(m,{p.second.IOIndex()*192,1024});
        } else if(p.second.isOutput()) {
            //qDebug() << p.second.id().c_str() << "is output";
            //mSystem.addVConstraint(m,-1024*SS);
            mSystem.addPConstrain(m,{p.second.IOIndex()*192,-1024});
        } else {
            mSystem.addVConstraint(m,1024-64*p.second.level());
        }


        mMasses[p.first] = m;
    }

    int segments = 1;

    for(const auto& p : graph->gates()) {
        const Node& g =  p.second;
        Mass* ms = mMasses[g.id()];

        float congr = std::pow(g.children().size(),2);

        //qDebug() << "for " << g.id().c_str() << "congr is" << congr;
        for(const Node::Child& c : g.children()) {
            EdgeItem* e = newEdge(segments);
            Mass* me = mMasses[c.node->id()];
            mSystem.addSpring(ms,me,sk/congr,l0*congr);

            ms->addMovable(e->getHandle(0));
            me->addMovable(e->getHandle(segments));
        }
    }
}
