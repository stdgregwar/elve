#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include <unordered_map>
#include <vector>
#include <set>

typedef std::vector<Node*> NodePtrs;
typedef std::unordered_map<NodeID,Node> NodesByID;
typedef std::set<NodeID> NodeNames;
//typedef std::vector<Node::Description> NodeDescrs;
typedef std::unordered_map<NodeID,Node::Description> NodeDescriptions;
typedef std::pair<NodeID,NodeID> Edge;
typedef std::vector<Edge> AdjacencyList;


class Graph
{
public:
    Graph();
    Graph(const NodeDescriptions& descrs, const AdjacencyList& edges);
    Node *addNode(const NodeID& id, Node::Type type, Index ioi = 0);
    Node *addNode(const Node::Description& des);
    void addEdge(const NodeID& from, const NodeID& to, bool invert = false);
    const NodesByID& nodes() const;
    size_t gateCount() const;
    SharedGraph clusterize(size_t maxLevel) const;
    SharedGraph group(const NodeNames& toGroup, NodeID groupID) const;
    SharedGraph merge(SharedGraph other) const;
    NodeID uniqueID(const NodeID& base) const;
    const NodePtrs& inputs();
    const NodePtrs& outputs();
    NodeDescriptions descriptions() const;
    AdjacencyList adjacencyList() const;
private:
    NodePtrs mInputs;
    NodePtrs mOutputs;
    NodesByID mNodes;
};

#endif // GRAPH_H
