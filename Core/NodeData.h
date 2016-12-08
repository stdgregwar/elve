#ifndef NODEDATA_H
#define NODEDATA_H

#include <string>
#include <set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <QJsonObject>

typedef std::string NodeID;
typedef unsigned Index;
typedef std::vector<NodeID> NodeIDs;
typedef QJsonObject NodeProperties;

enum NodeType{
    NODE,
    INPUT,
    OUTPUT,
    CLUSTER
};

class Node;
class NodeData
{
    friend class Node;
public:
    NodeData(const NodeID& id = "unnamed",const NodeIDs& dependencies = {}, const NodeType& type = NODE, const NodeProperties& props = {}, const Index& ioindex = 0);
    NodeData(const QJsonObject& obj);
    const NodeProperties& properties() const;
    NodeProperties& properties();
    const NodeID& id() const;
    const NodeIDs& dependencies() const;
    const NodeType& type() const;
    const Index& ioIndex() const;
    const QJsonObject json() const;
private:
    NodeProperties mProperties;
    NodeID mId;
    NodeIDs mDependencies;
    NodeType mType;
    Index mIOIndex;
};

typedef std::unordered_map<NodeID,NodeData> NodeDatas;

#endif // NODEDATA_H
