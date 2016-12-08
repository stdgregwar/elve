#include "Graph.h"
#include <string>
#include "Node.h"

#include <algorithm>
#include <QDebug>
#include <QJsonArray>
#include "utils.h"

using namespace std;

Graph::Graph(const SharedData &data) : mData(data)
{
    using pair_type = NodeDatas::value_type;
    for(const pair_type& p : data->nodeDatas()) {
        addNode(p.second);
    }
    for(const pair_type& p : data->nodeDatas()) {
        for(const NodeID& did : p.second.dependencies()) {
            addEdge(did,p.first);
        }
    }
}

const QString& Graph::filename() const {
    return mData->mFilename;
}

const NodesByID& Graph::nodes() const
{
    return mNodes;
}

size_t Graph::nodeCount() const {
    return mNodes.size();
}

const NodeID& Graph::alias(const NodeID& id) const {
    auto it = mAliases.find(id);
    if(it != mAliases.end()) return alias(it->second);
    return id;
}

Node* Graph::addNode(const NodeData& d) {
    auto pi = mNodes.emplace(d.id(),d);

    Node* n = &pi.first->second;
    if(d.type() == NodeType::OUTPUT) {
        mOutputs.push_back(n);
    } else if (d.type() == NodeType::INPUT) {
        mInputs.push_back(n);
    }
    return n;
}

void Graph::addEdge(const NodeID& from, const NodeID& to)
{
    mNodes.at(from).addChild(&mNodes.at(to));
}

SharedGraph Graph::clusterize(size_t maxLevel) const {
    return nullptr;
}

NodeID Graph::uniqueID(const NodeID& base) const {
    NodeID current = base;
    unsigned suffix = 1;
    repeat:
    for(const auto& p : mNodes) {
        const NodeID& id = p.first;
        if(id == current) {
           current = base + "_" + std::to_string(suffix++);
           goto repeat;
        }
    }
    return current;
}


SharedGraph Graph::ungroup(const NodeID &cluster) {

//    auto it = mNodes.find(cluster);
//    if(it == mNodes.end()) {
//        return shared_from_this();
//    }
//    const Node& cn = it->second;
//    if(!cn.getClusteredGraph()) { //Node is not a cluster
//        return shared_from_this();
//    }

//    const SharedGraph& cg = cn.getClusteredGraph();

//    NodeDescriptions gnodes; gnodes.reserve(nodeCount()+cg->nodeCount());
//    AdjacencyList gadj; gadj.reserve(gnodes.size()*2); //True for AIG

//    using pair_type = NodesByID::value_type;
//    for(const pair_type& p : nodes()) {
//        const Node& n = p.second;
//        if(n.id() != cluster) {
//            gnodes.emplace(n.id(),n);
//            for(const Node* an : n.ancestors()) { //Add all ancestors that are not part of the cluster
//                if(an->id() != cluster) {
//                    gadj.push_back({an->id(),n.id()});
//                }
//            }
//        }
//    }

//    for(const pair_type& p : cg->nodes()) {
//        const Node& n = p.second;
//        if(n.isOutput()) {
//            for(const Node* an : n.ancestors()) {//Add
//                gadj.push_back({an->id() ,alias(n.id())});
//            }
//        } else if(!n.isInput()) {
//            gnodes.emplace(n.id(),n);
//            for(const Node* an : n.ancestors()) { //Add all ancestors that are not part of the cluster
//                if(an->isInput()) { //Make sure node is connected to right node/cluster
//                    gadj.push_back({alias(an->id()),n.id()});
//                } else {
//                    gadj.push_back({an->id(),n.id()});
//                }
//            }
//        }
//    }

//    return make_shared<Graph>(gnodes,gadj,aliasesWithout(cluster));
    return SharedGraph();
}

SharedGraph Graph::group(const NodeIDs &toGroup, const NodeID &groupID) {
//    if(toGroup.size() < 2) {
//        return shared_from_this();
//    }

//    Node::Type t;
//    Index i;

//    NodeDescriptions clusteredNodes; clusteredNodes.reserve(toGroup.size());
//    AdjacencyList clusteredAdj; clusteredAdj.reserve(clusteredNodes.size()*2);

//    NodeDescriptions newNodes;
//    newNodes.reserve(mNodes.size()-toGroup.size()+1); //Reserve right number of nodes

//    AdjacencyList newAdj;
//    newAdj.reserve(newNodes.size()*2); //Reserve average number of nodes


//    size_t inputi = 0;
//    size_t outputi = 0;

//    for(const auto& p : mNodes) {
//        const NodeID& nid = p.first;
//        const Node& n = p.second;
//        if(toGroup.count(nid)) {
//            t = n.type();
//            i = n.IOIndex();
//            clusteredNodes.emplace(n.id(),n);
//            for(const Node* pan : n.ancestors()) {
//                const Node& an = *pan;
//                if(!toGroup.count(an.id())) { //If ancestor is external
//                    newAdj.push_back({an.id(),groupID});
//                    //Add external ancestor as input
//                    clusteredNodes.emplace(an.id(),
//                                           Node::Description(an.id(),Node::INPUT,an.properties(),inputi++));
//                }
//                clusteredAdj.push_back({an.id(),nid});
//            }
//            for(const Node* pcn : n.children()) {
//                const Node& cn = *pcn;
//                if(!toGroup.count(cn.id())) { //If child is external
//                    newAdj.push_back(Edge{groupID,cn.id()});
//                    //Add external ancestor as output
//                    clusteredAdj.push_back({nid,cn.id()}); //Connect child only if in outputs
//                    clusteredNodes.emplace(cn.id(),
//                                           Node::Description(cn.id(),Node::OUTPUT,cn.properties(),outputi++));
//                }
//            }
//        } else { //If node not in group reinsert it in graph
//            newNodes.emplace(n.id(),n);
//            for(const Node* pan : n.ancestors()) {
//                const Node& an = *pan;
//                if(toGroup.count(an.id()) == 0) { //If ancestor is also external
//                    newAdj.push_back({an.id(),nid});
//                }
//            }
//        }
//    }

//    newNodes.emplace(groupID,Node::Description(groupID,t == Node::NODE ? Node::CLUSTER : t,QJsonObject(),i));
//    newNodes.at(groupID).graph = make_shared<Graph>(clusteredNodes,clusteredAdj);
//    Aliases als(mAliases);
//    for(const NodeID& id : toGroup) {
//        als.emplace(id,groupID);
//    }
//    return make_shared<Graph>(newNodes,newAdj,als);
    return SharedGraph();
}

const NodePtrs& Graph::inputs() {
    return mInputs;
}

const NodePtrs& Graph::outputs() {
    return mOutputs;
}

size_t Graph::inputCount() const
{
    return mInputs.size();
}

size_t Graph::outputCount() const
{
    return mOutputs.size();
}

size_t Graph::maxInputIndex() const
{
    return (*std::max_element(mInputs.begin(),mInputs.end(),
        [](const Node* a,const Node* b) {
            return a->IOIndex() < b->IOIndex();
    }))->IOIndex();
}

size_t Graph::maxOutputIndex() const
{
    return (*std::max_element(mOutputs.begin(),mOutputs.end(),
        [](const Node* a,const Node* b) {
            return a->IOIndex() < b->IOIndex();
    }))->IOIndex();
}

QJsonObject Graph::json() const
{
    QJsonObject main;
    main.insert("graph_data",mData->json());

    //Todo insert clustering details here

    return main;
}

NodeLevel Graph::highestLevel() const {
    using pair_type = NodesByID::value_type;

    return max_element(mNodes.begin(),mNodes.end(),
        [](const pair_type& a, const pair_type& b)->bool {
            return a.second.level() < b.second.level();
        })->second.level();
}

SharedGraph Graph::fromJson(const QJsonObject& obj)
{
    SharedData sdata = make_shared<GraphData>(obj.value("graph_data").toObject());
    //Todo find clustering primitives here
    return make_shared<Graph>(sdata);
}

Aliases Graph::aliasesWithout(const NodeID& repl) const {
    Aliases als;
    for(const auto& p : mAliases) {
        if(p.second != repl) {
            als.insert(p);
        }
    }
    return als;
}
