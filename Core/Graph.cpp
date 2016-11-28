#include "Graph.h"
#include <string>
#include "Node.h"

#include <algorithm>
#include <QDebug>
#include <QJsonArray>
#include "utils.h"

using namespace std;

Graph::Graph()
{

}

Graph::Graph(const NodeDescriptions &descrs, const AdjacencyList& edges)
{
    for(const auto& p : descrs) {
        //qDebug() << p.second.properties;
        addNode(p.second);
    }
    for(auto p : edges) {
        addEdge(p.first,p.second);
    }
}

void Graph::setFilename(const string &filename) {
    mFilename = filename;
}

const std::string& Graph::filename() const {
    return mFilename;
}

const NodesByID& Graph::nodes() const
{
    return mNodes;
}

size_t Graph::nodeCount() const {
    return mNodes.size();
}

Node* Graph::addNode(const Node::Description& des) {
    auto pi = mNodes.emplace(std::piecewise_construct,
                             std::forward_as_tuple(des.id),
                             std::forward_as_tuple(des));

    Node* n = &pi.first->second;
    if(des.type == Node::OUTPUT) {
        mOutputs.push_back(n);
    } else if (des.type == Node::INPUT) {
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

    auto it = mNodes.find(cluster);
    if(it == mNodes.end()) {
        return shared_from_this();
    }
    const Node& cn = it->second;
    if(!cn.getClusteredGraph()) { //Node is not a cluster
        return shared_from_this();
    }

    NodeDescriptions gnodes = descriptions();
    AdjacencyList gadj = adjacencyList();
    NodeDescriptions cnodes = cn.getClusteredGraph()->descriptions();
    AdjacencyList cadj = cn.getClusteredGraph()->adjacencyList();

    gnodes.insert(cnodes.begin(),cnodes.end());


    for(const Edge& e : gadj) {
        if(e.first != cluster && e.second != cluster) {
            cadj.push_back(e);
        }
    }
    gnodes.erase(cluster);
    return make_shared<Graph>(gnodes,cadj);
}

SharedGraph Graph::group(const NodeNames &toGroup, const NodeID &groupID) {
    if(toGroup.size() < 2) {
        return shared_from_this();
    }

    Node::Type t;
    Index i;

    NodeDescriptions clusteredNodes; clusteredNodes.reserve(toGroup.size());
    AdjacencyList clusteredAdj; clusteredAdj.reserve(clusteredNodes.size()*2);

    NodeDescriptions newNodes;
    newNodes.reserve(mNodes.size()-toGroup.size()+1); //Reserve right number of nodes

    AdjacencyList newAdj;
    newAdj.reserve(newNodes.size()*2); //Reserve average number of nodes

    size_t inputi = 0;
    size_t outputi = 0;

    for(const auto& p : mNodes) {
        const NodeID& nid = p.first;
        const Node& n = p.second;
        if(toGroup.count(nid)) {
            t = n.type();
            i = n.IOIndex();
            clusteredNodes.emplace(n.id(),n);
            for(const Node* pan : n.ancestors()) {
                const Node& an = *pan;
                if(!toGroup.count(an.id())) { //If ancestor is external
                    newAdj.push_back({an.id(),groupID});
                    //Add external ancestor as input
                    clusteredNodes.emplace(an.id(),
                                           Node::Description(an.id(),Node::INPUT,an.properties(),inputi++));
                }
                clusteredAdj.push_back({an.id(),nid});
            }
            for(const Node* pcn : n.children()) {
                const Node& cn = *pcn;
                if(!toGroup.count(cn.id())) { //If child is external
                    newAdj.push_back(Edge{groupID,cn.id()});
                    //Add external ancestor as output
                    clusteredAdj.push_back({nid,cn.id()}); //Connect child only if in outputs
                    clusteredNodes.emplace(cn.id(),
                                           Node::Description(cn.id(),Node::OUTPUT,cn.properties(),outputi++));
                }
            }
        } else { //If node not in group reinsert it in graph
            newNodes.emplace(n.id(),n);
            for(const Node* pan : n.ancestors()) {
                const Node& an = *pan;
                if(toGroup.count(an.id()) == 0) { //If ancestor is also external
                    newAdj.push_back({an.id(),nid});
                }
            }
        }
    }

    newNodes.emplace(groupID,Node::Description(groupID,t == Node::NODE ? Node::CLUSTER : t,QJsonObject(),i));
    newNodes.at(groupID).graph = make_shared<Graph>(clusteredNodes,clusteredAdj);
    return make_shared<Graph>(newNodes,newAdj);
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


    //all node json
    QJsonArray nodeArray;

    //Adjacency list from ancestor to child
    QJsonObject adj;

    using pair_type = NodesByID::value_type;
    for(const pair_type& p : mNodes) {
        nodeArray.append(p.second.json());
        QJsonArray jancestors;
        for(const Node* an : p.second.ancestors()) {
            jancestors.append(QString::fromStdString(an->id()));
        }
        adj.insert(QString::fromStdString(p.first),jancestors);
    }
    main.insert("node_count",(int)mNodes.size());
    main.insert("nodes",nodeArray);
    main.insert("adjacency",adj);
    return main;
}

NodeLevel Graph::highestLevel() const {
    using pair_type = NodesByID::value_type;

    return max_element(mNodes.begin(),mNodes.end(),
        [](const pair_type& a, const pair_type& b)->bool {
            return a.second.level() < b.second.level();
        })->second.level();
}

NodeDescriptions Graph::descriptions() const {
    NodeDescriptions descrs; descrs.reserve(mNodes.size());
    for(const auto& p : mNodes) {
        descrs.emplace(p.first,p.second);
    }
    return descrs;
}

SharedGraph Graph::fromJson(const QJsonObject& obj)
{
    size_t count = obj.value("node_count").toInt();
    NodeDescriptions descrs; descrs.reserve(obj.value("node_count").toInt());
    AdjacencyList adjl; adjl.reserve(count*2); //Good guess I think

    QJsonArray nodeArray = obj.value("nodes").toArray();
    for(const QJsonValue& val : nodeArray) {
        QJsonObject nObj = val.toObject();
        descrs.emplace(std::piecewise_construct,
                       std::forward_as_tuple(nObj.value("id").toString("noname").toStdString()),
                       std::forward_as_tuple(nObj));
    }
    QJsonObject adj = obj.value("adjacency").toObject();
    for(QJsonObject::const_iterator it = adj.constBegin();
        it != adj.constEnd(); it++) {
        NodeID nid = it.key().toStdString();
        QJsonArray jancestors = it.value().toArray();
        for(const QJsonValue& v : jancestors) {
            NodeID aid = v.toString().toStdString();
            adjl.push_back({aid,nid});
        }
    }

    return make_shared<Graph>(descrs,adjl);
}

AdjacencyList Graph::adjacencyList() const {
    AdjacencyList l; l.reserve(mNodes.size()*2);
    for(const auto& p : mNodes) {
        const Node& n = p.second;
        for(const Node* an : n.ancestors()) {
            l.push_back({an->id(),p.first});
        }
    }
    return l;
}
