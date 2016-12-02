#ifndef NODEDATA_H
#define NODEDATA_H

#include <string>
#include <set>
#include <unordered_map>
#include <memory>
#include <QJsonObject>

typedef std::string NodeID;
typedef unsigned Index;
typedef std::set<NodeID> NodeIDs;

enum NodeType{
    NODE,
    INPUT,
    OUTPUT
};

class Node;
class NodeData
{
    friend class Node;
public:
    NodeData();
private:
    QJsonObject mProperties;
    NodeID mId;
    NodeIDs mDependencies;
    NodeType mType;
    Index mIOIndex;
};

typedef std::unordered_map<NodeID,NodeData> NodeDatas;

#endif // NODEDATA_H
