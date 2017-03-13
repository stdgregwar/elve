#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include <unordered_map>
#include <vector>
#include <set>

#include "GraphData.h"

namespace Elve {

typedef std::vector<Node*> NodePtrs;
typedef std::unordered_map<NodeID,Node> NodesByID;

struct Pin{
    Pin(const NodeID& id) : id(id),index(-1){}
    Pin(const NodeID &id,const Index& index) : id(id), index(index){}
    NodeID id;
    Index  index;
};

inline bool operator==(const Pin& a,const Pin& b);

struct Pin_hash {
    std::size_t operator () (const Pin& p) const {
            return p.id;
        }
};

typedef std::unordered_map<Pin,Pin,Pin_hash> Aliases;
typedef std::pair<NodeID,NodeID> Edge;
typedef std::vector<Edge> AdjacencyList;

class Graph : public std::enable_shared_from_this<Graph>
{
public:
    Graph(const SharedData& data);
    Graph(const SharedData& data, const SparseData &extraData, const Aliases& aliases, const NodeIDSet &excluded = {});

    const NodesByID& nodes() const;
    const Pin& alias(const Pin& id) const;
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

    const SharedData & datas() const;
    const SparseData& extraData() const;
    const Aliases& aliases() const;
    const NodeIDSet excluded() const;

    const NodeData& data(const NodeID& id) const;
    const Node& node(const NodeID& id) const;

    const NodePtrs& inputs();
    const NodePtrs& outputs();
    QJsonObject json() const;
    static SharedGraph fromJson(const QJsonObject &obj);
    const QString &filename() const;
private:
    Node* addNode(const NodeData &d);
    void addEdge(const NodeID& from, Index fi, const NodeID& to, Index ti);
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

}
#endif // GRAPH_H
