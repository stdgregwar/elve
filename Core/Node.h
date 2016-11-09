#ifndef GATE_H
#define GATE_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <QJsonObject>

class Node;

typedef std::shared_ptr<Node> SharedNode;
typedef std::weak_ptr<Node> WeakNode;
typedef std::string NodeID;
typedef unsigned Index;
typedef long NodeLevel;

class Graph;

typedef std::shared_ptr<Graph> SharedGraph;

class Node
{
public:


    enum Type{
        NODE,
        INPUT,
        OUTPUT,
        CLUSTER
    };

    struct Description {
        Description(const NodeID& aid,Type at,const QJsonObject& props = QJsonObject(),Index ai = 0) : id(aid), type(at), properties(props), ioi(ai) {}
        Description(const Node& n) : id(n.id()),type(n.type()),ioi(n.IOIndex()),properties(n.mProperties),graph(n.mGraph) {}
        Description(const QJsonObject& obj);
        QJsonObject properties;
        NodeID id;
        Type type;
        Index ioi;
        SharedGraph graph;
    };


    typedef std::vector<Node*> Children;
    typedef std::vector<Node*> Ancestors;

    Node(const NodeID& id, Type type, Index ioi = 0);
    Node(const Description& desc);
    void addChild(Node* child);
    void addAncestor(Node* anc);
    void setIOIndex(const Index& i);
    const Index& IOIndex() const;
    const Ancestors& ancestors() const;
    const Children& children() const;
    const NodeID& id() const {return mID;}
    size_t ancestorCount() const;
    size_t childCount() const;
    bool isInput() const;
    bool isOutput() const;
    const Type& type() const;
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

    QJsonObject mProperties;

    NodeID mID;
    Ancestors mAncestors;
    Children mChildren;
    Type mType;
    Index mIOindex;
};

#endif // GATE_H
