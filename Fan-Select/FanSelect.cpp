#include "FanSelect.h"
#include <GraphWidget.h>
#include <QtSvg/QSvgGenerator>
#include <QDebug>

using namespace Elve;

FanSelect::FanSelect()
{
    opts().add_options()
            ("in","select fanin cone(s)")
            ("out","select fanout cone(s)")
            ("inmask",po::value(&mInMask),"input selection mask")
            ("outmask",po::value(&mInMask),"output selection mask");
}

void _gatherDependencies(const Node* node, Selection& toFill) {
    for(Node* an : node->ancestors()) {
        toFill.add(an->id());
        _gatherDependencies(an,toFill);
    }
}

void _gatherDependents(const Node* node, Selection& toFill) {
    for(Node* an : node->children()) {
        toFill.add(an->id());
        _gatherDependents(an,toFill);
    }
}

SharedEGraph FanSelect::transform(const SharedEGraph& eg) {
    //Assuming graph is acyclic
    Selection conein;
    int inMask = eg->mask();
    if(is_set("inmask")) inMask = mInMask;
    int outMask = eg->mask();
    if(is_set("outmask")) outMask = mOutMask;
    Selection& inSel = eg->selection(inMask);
    if(is_set("in")) {
        for(const NodeID& prim : inSel) {
            const Node* n = &eg->graph()->nodes().at(prim);
            _gatherDependencies(n,conein);
        }
    }

    Selection coneout;
    if(is_set("out")) {
        for(const NodeID& prim : inSel) {
            const Node* n = &eg->graph()->nodes().at(prim);
            _gatherDependents(n,coneout);
        }
    }
    conein.add(coneout);
    //qDebug() << conein;
    eg->selection(outMask).clear();
    eg->selection(outMask).add(conein);
    return eg;
}
