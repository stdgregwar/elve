#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include <unordered_map>
#include <vector>
#include <set>

#include "GraphData.h"

typedef std::vector<Node*> NodePtrs;
typedef std::unordered_map<NodeID,Node> NodesByID;

typedef std::unordered_map<NodeID,NodeID> Aliases;
typedef std::pair<NodeID,NodeID> Edge;
typedef std::vector<Edge> AdjacencyList;

class Graph : public std::enable_shared_from_this<Graph>
{
public:
    Graph(const SharedData& data);
    Graph(const SharedData& data, const NodeIDSet& groups, const Aliases& aliases, const NodeIDSet &excluded = {});

    const NodesByID& nodes() const;
    const NodeID& alias(const NodeID& id) const;
    size_t nodeCount() const;
    SharedGraph clusterize(size_t maxLevel) const;
    SharedGraph group(const NodeIDSet& toGroup, const NodeID& groupID);
    SharedGraph ungroup(const NodeID& cluster);
    NodeName uniqueName(const NodeName &base) const;
    NodeLevel highestLevel() const;
    size_t inputCount() const;
    size_t outputCount() const;
    size_t maxInputIndex() const;
    size_t maxOutputIndex() const;

    const NodePtrs& inputs();
    const NodePtrs& outputs();
    QJsonObject json() const;
    static SharedGraph fromJson(const QJsonObject &obj);
    const QString &filename() const;
private:
    Node* addNode(const NodeData &d);
    void addEdge(const NodeID& from, const NodeID& to);
    Aliases aliasesWithout(const NodeID& repl) const;
    NodePtrs mInputs;
    NodePtrs mOutputs;
    NodesByID mNodes;
    Aliases mAliases;
    SharedData mData;
    NodeDatas  mGroupsData;
    NodeIDSet mExcluded;
};

#endif // GRAPH_H
