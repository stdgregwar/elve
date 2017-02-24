#include "EdgeLook.h"

EdgeLook::EdgeLook(const NodeLook &ancestor, const NodeLook &child) : mFrom(ancestor), mTo(child)
{
    //Find connexion indice in nodes
    for(const Node::Connexion& c : child.node().fanIn()) {
        if(&ancestor.node() == c.node) {
            mIFrom = c.from;
        }
    }
    for(const Node::Connexion& c : ancestor.node().fanOut()) {
        if(&child.node() == c.node) {
            mITo = c.to;
        }
    }
}

Index EdgeLook::iFrom() const {
    return mIFrom;
}

Index EdgeLook::iTo() const {
    return mITo;
}

const NodeLook& EdgeLook::from() const {
    return mFrom;
}

const NodeLook& EdgeLook::to() const {
    return mTo;
}
