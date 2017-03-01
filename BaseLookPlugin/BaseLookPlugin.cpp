#include "BaseLookPlugin.h"
#include <GraphWidget.h>
#include <QDebug>
#include "BlockNodeLook.h"
#include "BlockEdgeLook.h"

using namespace Elve;

BlockLookPlugin::BlockLookPlugin()
{

}

NodeLook* BlockLookPlugin::node(const Node &n) {
    return new BlockNodeLook(n);
}

EdgeLook* BlockLookPlugin::edge(const NodeLook &ancestor, const NodeLook &children) {
    return new BlockEdgeLook(ancestor,children);
}

OrientationHint BlockLookPlugin::orientationHint() const {
    return LEFTRIGHT;
}
