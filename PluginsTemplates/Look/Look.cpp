#include "Look.h" // TODO : change this to your actual header file

using namespace Elve;

LookPluginName::LookPluginName() {
    //load default colors
}

NodeLook* LookPluginName::node(const Node &n) {
    return nullptr; // TODO return your NodeLook subclass instance
}

EdgeLook* LookPluginName::edge(const NodeLook &ancestor, Index outi, const NodeLook &children, Index ini) {
    return nullptr; // TODO return your EdgeLook subclass instance
}

OrientationHint LookPluginName::orientationHint() const {
    return BOTTOMUP; // TODO Here say how do you want the layout to orient the graph
}
