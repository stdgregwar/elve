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
    Graph(const SharedData& data, const SparseData &extraData, const Aliases& aliases, const NodeIDSet &excluded = {});

    const NodesByID& nodes() const;
    const NodeID& alias(const NodeID& id) const;
    size_t nodeCount() const;
    SharedGraph clusterize(size_t level);
    SharedGraph group(const NodeIDSet& toGroup, const NodeID &i, const NodeName &groupName);
    SharedGraph ungroup(const NodeID& cluster);
    SharedGraph fastGroup(const std::vector<NodeIDSet>& groups, const NodeName& basename);
    NodeName uniqueName(const NodeName &base) const;
    NodeLevel highestLevel() const;
    size_t inputCount() const;
    size_t outputCount() const;
    size_t maxInputIndex() const;
    size_t maxOutputIndex() const;
    NodeID newID() const;
    const NodeData& data(const NodeID& id) const;

    const NodePtrs& inputs();
    const NodePtrs& outputs();
    QJsonObject json() const;
    static SharedGraph fromJson(const QJsonObject &obj);
    const QString &filename() const;
private:
    Node* addNode(const NodeData &d);
    void addEdge(const NodeID& from, const NodeID& to);
    Aliases aliasesWithout(const NodeID& repl) const;
    NodeIDSet excludedWithout(const Dependencies &ids) const;
    NodePtrs mInputs;
    NodePtrs mOutputs;
    NodesByID mNodes;
    Aliases mAliases;
    SharedData mData;
    SparseData mExtraData;
    NodeIDSet mExcluded;
    mutable NodeID mLastId;
};

#endif // GRAPH_H
