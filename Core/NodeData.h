#ifndef NODEDATA_H
#define NODEDATA_H

#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <QJsonObject>
#include <QJsonArray>

typedef unsigned NodeID;
typedef std::string Name;
typedef std::vector<std::string> Names;
typedef Name NodeName;
typedef unsigned Index;
typedef std::vector<NodeID> NodeIDs;
typedef Names NodeNames;
typedef std::unordered_set<NodeID> NodeIDSet;
typedef QJsonObject NodeProperties;

struct Dependency {
    Dependency(const NodeID& id);
    Dependency(const NodeID &id,Index from, Index to);
    Dependency(const QJsonArray& v);
    QJsonValue json() const;
    NodeID id;
    Index from;
    Index to;
};

typedef std::vector<Dependency> Dependencies;

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

#endif // NODEDATA_H
