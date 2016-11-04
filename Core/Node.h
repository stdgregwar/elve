#ifndef GATE_H
#define GATE_H

#include <memory>
#include <vector>
#include <string>

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
        Description(const NodeID& aid,Type at,Index ai = 0) : id(aid), type(at), ioi(ai) {}
        Description(const Node& n) : id(n.id()),type(n.type()),ioi(n.IOIndex()) {}
        NodeID id;
        Type type;
        Index ioi;
    };

    struct Child{
        Node* node;
        bool inverted;
    };

    typedef std::vector<Child> Children;
    typedef std::vector<Node*> Ancestors;

    Node(const NodeID& id, Type type, Index ioi = 0);
    void addChild(Node* child, bool inverted = false);
    void addAncestor(Node* anc, bool inverted = false);
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
private:
    void _addChild(Node* child, bool inverted);
    void _addAncestor(Node* anc);

    mutable NodeLevel mLevel;
    SharedGraph mGraph; //Intern graph when node is a cluster


    NodeID mID;
    Ancestors mAncestors;
    Children mChildren;
    Type mType;
    Index mIOindex;
};

#endif // GATE_H
