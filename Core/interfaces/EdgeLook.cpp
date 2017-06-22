#include "EdgeLook.h"
namespace Elve {
EdgeLook::EdgeLook(const NodeLook &ancestor, Index outi, const NodeLook &child, Index ini) :
    mFrom(ancestor), mTo(child), mIFrom(outi), mITo(ini)
{
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
}
