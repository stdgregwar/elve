#ifndef GATE_H
#define GATE_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <QJsonObject>

#include "NodeData.h"

class Node;

typedef std::shared_ptr<Node> SharedNode;
typedef std::weak_ptr<Node> WeakNode;
typedef std::string NodeID;
typedef std::vector<Node*> Nodes;
typedef unsigned Index;
typedef long NodeLevel;
class Graph;
typedef std::shared_ptr<Graph> SharedGraph;
class Node
{
public:
    typedef Nodes Children;
    typedef Nodes Ancestors;

    Node(const NodeID& id, NodeType type, Index ioi = 0);
    void addChild(Node* child);
    void addAncestor(Node* anc);
    void setIOIndex(const Index& i);
    const Index& IOIndex() const;
    const Ancestors& ancestors() const;
    const Children& children() const;
    const NodeID& id() const;
    size_t ancestorCount() const;
    size_t childCount() const;
    bool isInput() const;
    bool isOutput() const;
    const NodeType& type() const;
    const NodeLevel& level() const;
    const SharedGraph getClusteredGraph() const;
    void setClusteredGraph(SharedGraph graph);
    const QJsonObject& properties() const;
    QJsonObject json() const;
    //QJsonObject& properties();
private:
    void _addChild(Node* child);
    void _addAncestor(Node* anc);

    mutable NodeLevel mLevel;
    SharedGraph mGraph; //Intern graph when node is a cluster
    Ancestors mAncestors;
    Children mChildren;
    const NodeData* mData;
};

#endif // GATE_H
