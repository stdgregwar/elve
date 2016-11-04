#include "Node.h"
#include <algorithm>

Node::Node(const NodeID& id, Type type, Index ioi) : mID(id), mType(type), mIOindex(ioi), mLevel(-1)
{

}

void Node::addChild(Node* child)
{
    child->_addAncestor(this);
    _addChild(child);
}


void Node::addAncestor(Node* anc)
{
    anc->_addChild(this);
    _addAncestor(anc);
}

void Node::_addChild(Node* child)
{
    mChildren.push_back(child);
}

size_t Node::ancestorCount() const {
    return mAncestors.size();
}

size_t Node::childCount() const
{
    return mChildren.size();
}

void Node::setIOIndex(const Index& i)
{
    mIOindex = i;
}

const Index& Node::IOIndex() const
{
    return mIOindex;
}

void Node::_addAncestor(Node* anc)
{
    mAncestors.push_back(anc);
}

const Node::Ancestors& Node::ancestors() const
{
    return mAncestors;
}

bool Node::isInput() const
{
    return mType == INPUT;
}

bool Node::isOutput() const
{
    return mType == OUTPUT;
}

const Node::Type& Node::type() const
{
    return mType;
}

const NodeLevel& Node::level() const { //TODO add special treatment for OUTPUTS
    if(mType == INPUT) { //Base case
        return mLevel = 0;
    }
    if(mLevel == -1) { //Recursive def
        mLevel = (*std::max_element(mAncestors.begin(),mAncestors.end(),
            [](const Node* a,const Node* b) {
                return a->level() < b->level();
        }))->level() + 1;
    }
    return mLevel;
}

const QJsonObject& Node::properties() const {
    return mProperties;
}

const Node::Children& Node::children() const
{
    return mChildren;
}

