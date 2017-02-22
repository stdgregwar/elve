#include "BaseLookPlugin.h"
#include <GraphWidget.h>
#include <QDebug>
#include "BlockNodeLook.h"

BaseLookPlugin::BaseLookPlugin()
{

}

NodeLook* BaseLookPlugin::node(const Node &n) {
    return new BlockNodeLook(n);
}

EdgeLook* BaseLookPlugin::edge(const NodeLook &ancestor, const NodeLook &children) {

}
