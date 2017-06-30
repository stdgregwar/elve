#include "Node.h"
#include <algorithm>
#include <QJsonArray>
#include <QHash>
#include <Graph.h>

#include "utils.h"

namespace Elve {
Node::Node(const NodeData &data) : mData(data),mLevel(-1)
{

}

const NodeName& Node::name() const {
    return mData.name();
}

void Node::addChild(Node* child,  Index from, Index to)
{
    child->_addAncestor(this,from,to);
    _addChild(child,from,to);
}


void Node::addAncestor(Node* anc, Index from, Index to)
{
    anc->_addChild(this,from,to);
    _addAncestor(anc,from,to);
}

size_t Node::ancestorCount() const {
    return mAncestors.size();
}

size_t Node::childCount() const
{
    return mChildren.size();
}

const Index& Node::IOIndex() const
{
    return mData.ioIndex();
}

void Node::_addChild(Node* child, Index from, Index to)
{
    lazy_add(mChildren,child);
    mFanOut.push_back({child,from,to});
}

void Node::_addAncestor(Node* anc,  Index from, Index to)
{
    lazy_add(mAncestors,anc);
    mFanIn.push_back({anc,from,to});
}

const Node::Ancestors& Node::ancestors() const
{
    return mAncestors;
}

bool Node::isInput() const
{
    return mData.type() == INPUT or mData.type() == INPUT_CLUSTER;
}

bool Node::isOutput() const
{
    return mData.type() == OUTPUT or mData.type() == OUTPUT_CLUSTER;
}

const NodeType &Node::type() const
{
    return mData.type();
}

bool Node::isCluster() const {
    return type() == CLUSTER || type() == INPUT_CLUSTER || type() == OUTPUT_CLUSTER;
}

const NodeLevel& Node::level() const { //TODO add special treatment for OUTPUTS
    if(type() == INPUT) { //Base case
        return mLevel = 0;
    }
    if(mLevel == -1) { //Recursive def
        if(properties().contains("level")) {
            mLevel = properties().value("level").toInt();
        } else {
            for(const Node* an : ancestors()) {
                if(an->level() +1 > mLevel) {
                    mLevel = an->level() + 1;
                }
            }
        }
    }
    return mLevel;
}

const SharedGraph Node::getClusteredGraph() const
{
    return mGraph;
}

void Node::setClusteredGraph(SharedGraph graph)
{
    mGraph = graph;
}

int Node::inputCount() const {
    return mData.inputCount();
}

int Node::outputCount() const {
    return mData.outputCount();
}

Name Node::inputName(Index i) const {
    return mData.inputName(i);
}

Name Node::outputName(Index i) const {
    return mData.outputName(i);
}

const Node::Fan& Node::fanIn() const {
    return mFanIn;
}

const Node::Fan& Node::fanOut() const {
    return mFanOut;
}

const QJsonObject& Node::properties() const {
    return mData.properties();
}

const NodeData& Node::data() const {
    return mData;
}

const NodeID& Node::id() const {
    return mData.id();
}

const Node::Children& Node::children() const
{
    return mChildren;
}
}
