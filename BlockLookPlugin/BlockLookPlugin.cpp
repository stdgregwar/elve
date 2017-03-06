#include "BlockLookPlugin.h"
#include <GraphWidget.h>
#include <QDebug>
#include "BlockNodeLook.h"
#include "BlockEdgeLook.h"

using namespace Elve;

BasicLookPlugin::BasicLookPlugin()
{

}

NodeLook* BasicLookPlugin::node(const Node &n) {
    return new BasicNodeLook(n);
}

EdgeLook* BasicLookPlugin::edge(const NodeLook &ancestor, const NodeLook &children) {
    return new BasicEdgeLook(ancestor,children);
}

OrientationHint BasicLookPlugin::orientationHint() const {
    return LEFTRIGHT;
}
