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

/**
 * @brief Pin structure serving as id in the graph for connexions
 */
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

/**
 * @brief The main class in elve, describing the graph and the numerous
 * transformation that you can apply to it.
 *
 * Graph is the abstract part of the Graph, it contains no information about
 * layouting or drawing. It only stores the properties of the nodes and their
 * relation to each other as a pointer-based graph structure that speed-up
 * the access to GraphData. And exposes nodes and connexions in a convenient way.
 *
 *
 * The pointer based representation is expanded from the GraphData and auxiliary
 * data passed to the graph that controls how the graph expands :
 * - extraData : add Nodes to the graph
 * - aliases : maps Pin to Pin to redirect edges to other nodes, especially extra Nodes
 * - exluded : set of excluded Id's that prevent nodes being expanded
 *
 * Layout, Selection, Looks... are stored and managed in ExtendedGraph
 */
class Graph : public std::enable_shared_from_this<Graph>
{
public:
    /**
     * @brief construct the Graph from shared GraphData
     * @param data
     */
    Graph(const SharedData& data);

    /**
     * @brief construct the Graph from shared GraphData and extra parameters
     * @param data shared GraphData
     * @param extraData Sparse collection of NodeData for extra nodes added on the fly
     * @param aliases mapping from Pin to Pin that redirect edges
     * @param exluded set of exluded points
     */
    Graph(const SharedData& data, const SparseData &extraData, const Aliases& aliases, const NodeIDSet &excluded = {});

    /**
     * @brief get the map from NodeID to Nodes
     * @return the map
     */
    const NodesByID& nodes() const;

    /**
     * @brief get alias Pin for a particular Pin
     * @param id the desired Pin
     * @return the aliased Pin
     *
     * This resolve aliases recursively, so if there is a pin aliased to another
     * one that is also aliased you'll get the last one
     */
    const Pin alias(const Pin& id) const;

    /**
     * @brief expanded nodes count
     * @return
     */
    size_t nodeCount() const;

    /**
     * @brief apply a greedy algorithm to simplify the graph
     * @param level \deprecated{number of passes}
     * @return a new graph with the clusters grouped
     */
    SharedGraph clusterize(size_t level);

    /**
     * @brief group several nodes as only one node
     * @param toGroup ids of node to group
     * @param i id of the new node
     * @param groupName name of the new node
     * @return new graph with updated group
     */
    SharedGraph group(const NodeIDSet& toGroup, const NodeID &i, const NodeName &groupName);

    /**
     * @brief ungroup the selected group back to the initial nodes
     * @param cluster id of the group
     * @return a new graph with the cluster ungrouped
     */
    SharedGraph ungroup(const NodeID& cluster);

    /**
     * @brief group several set of node in a fast way
     * @param groups array of sets of node ids that each form a group
     * @param basename base name for the groups
     * @return a new graph with all groups updated
     *
     * This way created to allow to group multiple cluster at once without
     * re-expanding the graph, this allow very fast clustering.
     */
    SharedGraph fastGroup(const std::vector<NodeIDSet>& groups, const NodeName& basename);

    /**
     * @brief generate a unique name from a basename
     * @param base base name
     * @return a new name suffixed with a number if base already taken
     */
    NodeName uniqueName(const NodeName &base) const;

    /**
     * @brief the max of all nodes levels
     * @return
     */
    NodeLevel highestLevel() const;

    /**
     * @brief number of inputs
     * @return
     */
    size_t inputCount() const;

    /**
     * @brief number of outputs
     * @return
     */
    size_t outputCount() const;

    /**
     * @brief max index of inputs
     * @return
     */
    size_t maxInputIndex() const;

    /**
     * @brief max index of outputs
     * @return
     */
    size_t maxOutputIndex() const;

    /**
     * @brief return a new unique (in this graph) node ID
     * @return
     */
    NodeID newID() const;

    /**
     * @brief get the underlying shared GraphData
     * @return
     */
    const SharedData & datas() const;

    /**
     * @brief get the extra data used in this graph
     * @return
     */
    const SparseData& extraData() const;

    /**
     * @brief get the alias table of this graph
     * @return
     */
    const Aliases& aliases() const;

    /**
     * @brief get the excluded set from this graph
     * @return
     */
    const NodeIDSet excluded() const;

    /**
     * @brief get NodeData for a particular node, including extra nodes
     * @param id id of the Node
     * @return
     */
    const NodeData& data(const NodeID& id) const;

    /**
     * @brief get the Node from its id
     * @param id id of the Node
     * @return
     */
    const Node& node(const NodeID& id) const;

    /**
     * @brief get array of Node* containing all inputs
     * @return
     */
    const NodePtrs& inputs();

    /**
     * @brief get array of Node* containing all outputs
     * @return
     */
    const NodePtrs& outputs();

    /**
     * @brief construct the json representation of the Graph
     * @return
     */
    QJsonObject json() const;

    /**
     * @brief make a new shared graph from the given json
     * @param obj json Objet
     * @return a new Shared graph
     */
    static SharedGraph fromJson(const QJsonObject &obj);

    /**
     * @brief get the original filename of the graph
     * @return
     */
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
