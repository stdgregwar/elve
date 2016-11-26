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


class Graph : public std::enable_shared_from_this<Graph>
{
public:
    Graph();
    Graph(const NodeDescriptions& descrs, const AdjacencyList& edges);
    void setFilename(const std::string& filename);
    Node *addNode(const Node::Description& des);
    void addEdge(const NodeID& from, const NodeID& to);
    const NodesByID& nodes() const;
    size_t nodeCount() const;
    SharedGraph clusterize(size_t maxLevel) const;
    SharedGraph group(const NodeNames& toGroup, const NodeID& groupID);
    SharedGraph ungroup(const NodeID& cluster);
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
    const std::string& filename() const;
private:
    NodePtrs mInputs;
    NodePtrs mOutputs;
    NodesByID mNodes;
    std::string mFilename;
};

#endif // GRAPH_H
