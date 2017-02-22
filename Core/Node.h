#ifndef GATE_H
#define GATE_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <QJsonObject>

#include "NodeData.h"

class Node;

//typedef std::shared_ptr<Node> SharedNode;
//typedef std::weak_ptr<Node> WeakNode;

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

    struct Connexion {
        Node* node;
        Index from;
        Index to;
    };

    typedef std::vector<Connexion> Fan;

    Node(const NodeData& data);
    void addChild(Node* child, Index from, Index to);
    void addAncestor(Node* anc, Index from, Index to);
    const Index& IOIndex() const;
    const Ancestors& ancestors() const;
    const Children& children() const;
    const NodeID& id() const;
    const NodeName& name() const;
    size_t ancestorCount() const;
    size_t childCount() const;
    int inputCount() const;
    int outputCount() const;
    Name inputName(Index i) const;
    Name outputName(Index i) const;
    const Fan& fanIn() const;
    const Fan& fanOut() const;
    bool isInput() const;
    bool isOutput() const;
    const NodeType& type() const;
    bool isCluster() const;
    const NodeLevel& level() const;
    const SharedGraph getClusteredGraph() const;
    void setClusteredGraph(SharedGraph graph);
    const QJsonObject& properties() const;
    QJsonObject json() const;
    const NodeData& data() const;
    //QJsonObject& properties();
private:
    void _addChild(Node* child,  Index from, Index to);
    void _addAncestor(Node* anc, Index from, Index to);

    mutable NodeLevel mLevel;
    SharedGraph mGraph; //Intern graph when node is a cluster
    Ancestors mAncestors;
    Children mChildren;
    Fan mFanIn;
    Fan mFanOut;
    const NodeData& mData;
};

#endif // GATE_H
