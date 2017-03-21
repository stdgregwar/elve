#include "Extract.h"
#include <GraphWidget.h>
#include <QDebug>
#include <Node.h>

using namespace Elve;

using namespace std;

Extract::Extract()
{
    opts().add_options()
            ("m,mask",po::value(&mMask),"input mask");
}

SharedEGraph Extract::transform(const SharedEGraph& eg) {
    //Assuming graph is acyclic
    SharedGraph g = eg->graph();
    NodeIDSet excluded = g->excluded();
    int mask = is_set("mask") ? mMask : eg->mask();
    Selection isel = eg->invertedSelection(mask);
    Selection sel = eg->selection(mask);
    Aliases als = g->aliases();
    SparseData extraData = g->extraData();

    set<NodeID> inputDone;
    set<NodeID> outputDone;

    Index iindex = 0;
    Index oindex = 0;

    for(const NodeID& id : sel) {
        const Node& n = g->node(id);
        for(const Node::Connexion& c : n.fanIn()) {
            const NodeID& inid = c.node->id();
            if(!sel.count(inid) && !inputDone.count(inid)) { //Node is outside
                NodeID newId = g->newID();
                extraData.emplace(newId,
                                  c.node->data().asInput(newId,iindex++));
                inputDone.insert(inid);
                //aliases
                for(const Node::Connexion& oc : c.node->fanOut()) {
                    als.emplace(Pin(inid,oc.from),Pin(newId,oc.from));
                }
            }
        }
        for(const Node::Connexion& c : n.fanOut()) {
            const NodeID& outid = c.node->id();
            if(!sel.count(outid) && !outputDone.count(outid)) { //Node is outside
                NodeID newId = g->newID();
                extraData.emplace(newId,
                                  c.node->data().asOutput(newId,oindex++));
                outputDone.insert(outid);
                //aliases
                for(const Node::Connexion& ic : c.node->fanIn()) {
                    als.emplace(Pin(outid,ic.to),Pin(newId,ic.to));
                }
            }
        }
    }
    excluded.insert(isel.begin(),isel.end());
    SharedGraph newGraph = std::make_shared<Graph>(g->datas(),extraData,als,excluded);
    return eg->clone(newGraph);
}
