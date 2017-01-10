#include "EdgeLook.h"

EdgeLook::EdgeLook(NodeLook *ancestor, NodeLook *child) : mAncestor(ancestor), mChild(child)
{

}

NodeLook* EdgeLook::ancestor() {
    return mAncestor;
}

NodeLook *EdgeLook::child() {
    return mChild;
}
