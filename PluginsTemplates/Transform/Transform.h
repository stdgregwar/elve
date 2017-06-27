#pragma once

#include <interfaces/GraphTransformPlugin.h>

/**
 * @brief The Transform classes are used by the engine to modify the graph
 *
 *
 * Certainly the first type of plugin you would like to implement, the engine just calls the
 * overriden transform method with a graph and you return a new graph modified in any way.
 *
 * Note that the engine will apply different graph management policies depending of the type of your transform
 * the type is specified in the ELVE_TRANSFORM macro bellow
 */
class TransformName : public Elve::TransformPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.TransformName")
    Q_INTERFACES(Elve::TransformPlugin)
public:
    ELVE_TRANSFORM(TransformName,"GUI title","cmd_line_name",Elve::OTHER) //Last is the transform type
    TransformName();
    Elve::SharedEGraph transform(const Elve::SharedEGraph& eg) override;
};
