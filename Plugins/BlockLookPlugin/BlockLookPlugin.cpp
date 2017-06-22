#include "BlockLookPlugin.h"
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

EdgeLook* BlockLookPlugin::edge(const NodeLook &ancestor, Index outi, const NodeLook &children, Index ini) {
    return new BlockEdgeLook(ancestor,outi,children,ini);
}

OrientationHint BlockLookPlugin::orientationHint() const {
    return LEFTRIGHT;
}
