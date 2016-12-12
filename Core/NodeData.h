#ifndef NODEDATA_H
#define NODEDATA_H

#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <QJsonObject>

typedef unsigned NodeID;
typedef std::string NodeName;
typedef unsigned Index;
typedef std::vector<NodeID> NodeIDs;
typedef std::vector<NodeName> NodeNames;
typedef std::unordered_set<NodeID> NodeIDSet;
typedef QJsonObject NodeProperties;

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
    NodeData(const NodeID& id = -1, const NodeName& name = "unnamed",const NodeIDs& dependencies = {},
             const NodeType& type = NODE, const NodeProperties& props = {}, const Index& ioindex = 0);
    NodeData(const QJsonObject& obj);
    const NodeProperties& properties() const;
    NodeProperties& properties();
    const NodeID& id() const;
    const NodeIDs& dependencies() const;
    const NodeType& type() const;
    const Index& ioIndex() const;
    const QJsonObject json() const;
    const NodeName& name() const;
private:
    NodeProperties mProperties;
    NodeID mId;
    NodeName mName;
    NodeIDs mDependencies;
    NodeType mType;
    Index mIOIndex;
};

typedef std::vector<NodeData> NodeDatas;
typedef std::unordered_map<NodeID,NodeData> SparseData;

#endif // NODEDATA_H
