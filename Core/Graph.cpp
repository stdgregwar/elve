#include "Graph.h"
#include <string>
#include "Node.h"

#include <algorithm>
#include <QDebug>
#include <QJsonArray>

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

void Graph::setFilename(const QString& filename) {
    mFilename = filename;
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

SharedGraph Graph::merge(SharedGraph other) const {

}

SharedGraph Graph::group(const NodeNames &toGroup, NodeID groupID) const {
    NodeDescriptions newNodes;
    newNodes.reserve(mNodes.size()-toGroup.size()+1); //Reserve right number of nodes
    newNodes.emplace(groupID,Node::Description(groupID,Node::Type::CLUSTER));
    AdjacencyList newAdj;
    newAdj.reserve(newNodes.size()*2); //Reserve average number of nodes

    for(const auto& p : mNodes) {
        const NodeID& nid = p.first;
        const Node& n = p.second;
        if(toGroup.count(nid) > 0) {

            for(const Node* pan : n.ancestors()) {
                const Node& an = *pan;
                if(toGroup.count(an.id()) == 0) { //If ancestor is external
                    newAdj.push_back(Edge{an.id(),groupID});
                }
            }
            for(const Node* pcn : n.children()) {
                const Node& cn = *pcn;
                if(toGroup.count(cn.id()) == 0) { //If child is external
                    newAdj.push_back(Edge{groupID,cn.id()});
                }
            }
        } else { //If node not in group reinsert it in graph
            newNodes.emplace(n.id(),n);
            for(const Node* pan : n.ancestors()) {
                const Node& an = *pan;
                if(toGroup.count(an.id()) == 0) { //If ancestor is also external
                    newAdj.push_back(Edge{an.id(),nid});
                }
            }
        }
    }
    return SharedGraph(new Graph(newNodes,newAdj));
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
    for(const auto& p : mNodes) {
        //TODO
    }
}
