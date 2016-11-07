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
    void setFilename(const QString& filename);
    Node *addNode(const Node::Description& des);
    void addEdge(const NodeID& from, const NodeID& to);
    const NodesByID& nodes() const;
    size_t nodeCount() const;
    SharedGraph clusterize(size_t maxLevel) const;
    SharedGraph group(const NodeNames& toGroup, NodeID groupID) const;
    SharedGraph merge(SharedGraph other) const;
    NodeID uniqueID(const NodeID& base) const;
    NodeLevel highestLevel() const;
    size_t inputCount() const;
    size_t outputCount() const;
    const NodePtrs& inputs();
    const NodePtrs& outputs();
    NodeDescriptions descriptions() const;
    AdjacencyList adjacencyList() const;
    QJsonObject json() const;
    static SharedGraph fromJson(const QJsonObject &obj);
private:
    NodePtrs mInputs;
    NodePtrs mOutputs;
    NodesByID mNodes;
    QString mFilename;
};

#endif // GRAPH_H
