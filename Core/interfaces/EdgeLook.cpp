#include "EdgeLook.h"

EdgeLook::EdgeLook(NodeLook *ancestor, NodeLook *child) : mAncestor(ancestor), mChild(child)
{

}

NodeLook* EdgeLook::from() {
    return mAncestor;
}

NodeLook *EdgeLook::to() {
    return mChild;
}
