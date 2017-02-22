#pragma once

#include <interfaces/LookFactoryPlugin.h>

class BaseLookPlugin : public LookFactoryPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.baselook")
    Q_INTERFACES(LookFactoryPlugin)
public:
    ELVE_LOOK(BaseLookPlugin,"Basic","basic")
    NodeLook* node(const Node& n) override;
    EdgeLook* edge(const NodeLook& ancestor, const NodeLook& children) override;
};
