#include "Look.h"
#include <GraphWidget.h>
#include <QDebug>
#include <QDomDocument>
#include <QRegularExpression>
#include <QFile>

using namespace Elve;

LookPluginName::LookPluginName() {
    //load default colors
}

NodeLook* LookPluginName::node(const Node &n) {
    return nullptr;
}

EdgeLook* LookPluginName::edge(const NodeLook &ancestor, const NodeLook &children) {
    return nullptr;
}

OrientationHint LookPluginName::orientationHint() const {
    return BOTTOMUP; //Here say how do you want the layout to orient the graph
}
