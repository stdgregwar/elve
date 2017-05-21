#ifndef NODEDATA_H
#define NODEDATA_H

#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <QJsonObject>
#include <QJsonArray>

namespace Elve {

/**
 * @brief Node id type is a unique identifier for nodes
 *
 * Nodes are stored in many places in Elve and cannot always be
 * differentiated by their names, the introduction of this NodeID
 * type allow to speed up table lookup and graph random access.
 */
typedef unsigned NodeID;
typedef std::string Name;
typedef std::vector<std::string> Names;

/**
 * @brief Name of a node
 */
typedef Name NodeName;

/**
 * @brief typedef for node'input/output index
 */
typedef int Index;

/**
 * @brief array of ordered NodeIDs
 */
typedef std::vector<NodeID> NodeIDs;

/**
 * @brief NodeNames
 */
typedef Names NodeNames;

/**
 * @brief a fast hash set of NodeID 's
 */
typedef std::unordered_set<NodeID> NodeIDSet;

/**
 * @brief Attached properties bound to the node
 */
typedef QJsonObject NodeProperties;

/**
 * @brief Node dependency
 *
 * Represent direct ancestor of the node. I.e. one node of the fan-in
 *
 * It allows to specifie in which input the ancestor is connected an from
 * wich output of the ancestor the connexion comes.
 */
struct Dependency {
    Dependency(const NodeID& id);
    Dependency(const NodeID &id,Index from, Index to);
    Dependency(const QJsonArray& v);
    QJsonValue json() const;
    NodeID id;
    Index from;
    Index to;
};

/**
 * @brief NodeData fan-in
 */
typedef std::vector<Dependency> Dependencies;

/**
 * @brief Enum to discriminate node types
 */
enum NodeType{
    NODE,
    INPUT,
    OUTPUT,
    CLUSTER,
    INPUT_CLUSTER,
    OUTPUT_CLUSTER
};

class Node;
class NodeData
{
    friend class Node;
public:
    NodeData(const NodeID& id = -1, const NodeName& name = "unnamed",const Dependencies& dependencies = {},
             const NodeType& type = NODE, const NodeProperties& props = {}, const Index& ioindex = 0,
             int inputCount = 2, int outputCount = 1, const Names& inputNames = {}, const Names& outputNames = {});

    NodeData(const QJsonObject& obj);
    const NodeProperties& properties() const;
    NodeProperties& properties();
    const NodeID& id() const;
    const Dependencies &dependencies() const;
    const NodeType& type() const;
    const Index& ioIndex() const;
    const QJsonObject json() const;
    const NodeName& name() const;
    int inputCount() const;
    int outputCount() const;
    Name inputName(const Index& i) const;
    Name outputName(const Index& i) const;
    NodeData asInput(const NodeID& newId, const Index &io) const ;
    NodeData asOutput(const NodeID& newID, const Index& io) const;
private:
    NodeProperties mProperties;
    NodeID mId;
    NodeName mName;
    Names mInputNames;
    Names mOutputNames;
    size_t mInputCount;
    size_t mOutputCount;
    Dependencies mDependencies;
    NodeType mType;
    Index mIOIndex;
};

typedef std::vector<NodeData> NodeDatas;
typedef std::unordered_map<NodeID,NodeData> SparseData;

}

#endif // NODEDATA_H
