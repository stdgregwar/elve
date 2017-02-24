#include "BaseLookPlugin.h"
#include <GraphWidget.h>
#include <QDebug>
#include "BlockNodeLook.h"
#include "BlockEdgeLook.h"

BaseLookPlugin::BaseLookPlugin()
{

}

NodeLook* BaseLookPlugin::node(const Node &n) {
    return new BlockNodeLook(n);
}

EdgeLook* BaseLookPlugin::edge(const NodeLook &ancestor, const NodeLook &children) {
    return new BlockEdgeLook(ancestor,children);
}

OrientationHint BaseLookPlugin::orientationHint() const {
    return LEFTRIGHT;
}
