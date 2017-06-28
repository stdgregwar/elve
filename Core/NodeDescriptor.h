#pragma once

#include "Node.h"
#include "ExtendedGraph.h"

namespace Elve {

class NodeDescriptor
{
public:
    NodeDescriptor();
    static QString multipleDescription(const Elve::Selection& sel, const SharedEGraph &eg);
    static QString nodeDescription(const Elve::Node& node);
};

}
