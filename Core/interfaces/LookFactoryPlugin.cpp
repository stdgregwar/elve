#include "LookFactoryPlugin.h"

namespace Elve {

LookFactoryPlugin::LookFactoryPlugin()
{
}

LookFactoryPlugin::LookFactoryPlugin(const LookFactoryPlugin& other) :
    QObject(other.parent())
{
}

OrientationHint LookFactoryPlugin::orientationHint() const {
    return BOTTOMUP;
}

NodeLook* LookFactoryPlugin::getNode(const Node& n) {
    return node(n);
}

EdgeLook* LookFactoryPlugin::getEdge(const NodeLook& ancestor, const NodeLook& children) {
    return edge(ancestor,children);
}
}
