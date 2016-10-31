#include "Graph.h"
#include <string>
#include "Node.h"

Graph::Graph()
{

}

Graph::Graph(const NodeDescriptions &descrs, const AdjacencyList& edges)
{
    for(const auto& p : descrs) {
        addNode(p.second);
    }
    for(auto p : edges) {
        addEdge(p.first,p.second);
    }
}

const NodesByID& Graph::nodes() const
{
    return mNodes;
}

size_t Graph::gateCount() const {
    return mNodes.size();
}

Node* Graph::addNode(const NodeID& id, Node::Type type, Index ioi)
{
    auto it = mNodes.find(id);
    if(it != mNodes.end()) {
        return &it->second;
    } else {
        auto pi = mNodes.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(id),
                                 std::forward_as_tuple(id, type,ioi));
        return &pi.first->second;
    }
}

Node* Graph::addNode(const Node::Description& des) {
    return addNode(des.id,des.type,des.ioi);
}

void Graph::addEdge(const NodeID& from, const NodeID& to, bool invert)
{
    mNodes.at(from).addChild(&mNodes.at(to),invert);
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
            for(const Node::Child& cc : n.children()) {
                const Node& cn = *cc.node;
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

NodeDescriptions Graph::descriptions() const {
    NodeDescriptions descrs; descrs.reserve(mNodes.size());
    for(const auto& p : mNodes) {
        descrs.emplace(p.first,p.second);
    }
    return descrs;
}

AdjacencyList Graph::adjacencyList() const {
    for(const auto& p : mNodes) {

    }
}
