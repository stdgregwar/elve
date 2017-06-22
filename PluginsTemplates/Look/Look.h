#pragma once

#include <interfaces/LookFactoryPlugin.h>
#include <QPair>
#include <QSvgRenderer>
#include <QColor>
#include <QString>

/**
 * @brief Look plugins follow the factory pattern
 *
 *
 * The engine will use the derived factory to generate node and edge looks,
 * so of course when you subclass theses you want to also subclass Elve::EdgeLook
 * and Elve::NodeLook to be able to return them in the node and edge overriden methods
 * see BasicNodeLook for a concrete example
 */
class LookPluginName : public Elve::LookFactoryPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.LookPluginName")
    Q_INTERFACES(Elve::LookFactoryPlugin)
public:
    ELVE_LOOK(LookPluginName,"Name in the GUI","name_in_the_cli")
    Elve::NodeLook* node(const Elve::Node& n) override;
    Elve::EdgeLook* edge(const Elve::NodeLook& ancestor,Elve::Index outi, const Elve::NodeLook& children, Elve::Index ini) override;
    Elve::OrientationHint orientationHint() const override;
};
