#include "NodeData.h"

NodeData::NodeData(const NodeID& id,const NodeIDs& dependencies, const NodeType& type, const NodeProperties& props, const Index& ioindex)
    : mId(id),
      mDependencies(dependencies),
      mType(type),
      mProperties(props),
      mIOIndex(ioindex)
{

}

NodeProperties &NodeData::properties() {
    return mProperties;
}

const NodeProperties &NodeData::properties() const {
    return mProperties;
}

const NodeID& NodeData::id() const {
    return mId;
}

const NodeIDs& NodeData::dependencies() const {
    return mDependencies;
}

const NodeType& NodeData::type() const {
    return mType;
}

const Index& NodeData::ioIndex() const {
    return mIOIndex;
}
