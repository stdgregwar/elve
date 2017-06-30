#ifndef GRAPHTRANSFORM_H
#define GRAPHTRANSFORM_H

#include <ExtendedGraph.h>
#include "Plugin.h"

#define TransformPlugin_iid "ch.epfl.lap.elfe.TransformPlugin"

namespace Elve {

/**
 * @brief The TransformType enum
 *
 * State what the transform is modifying
 */
enum TransformType {
    SELECTION, ///Transform only affect selection, meaning graph is preserved
    OTHER ///Transform could affect anything
};

/**
 * @brief The GraphTransformPlugin abstract class
 *
 * This class is A QPlugin interface allowing to define new graph's transformation
 * in elve. Here transformation can mean anything from clustering to altering
 * selection masks.
 *
 * Since graphs are supposed to be immutable in elve, the transform should not
 * modify given graph but output a new modified copy of it. Except for selection
 * that are a mutable part.
 */
class TransformPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    /**
     * @brief transform the given graph in another
     * @param graph
     * @return
     *
     * Re-implement this in your plugin.
     *
     * Here should be the transform logic. Pay attention to stay in the limit
     * of the transform type you specified. For example, if the type is SELECTION
     * then return the given graph unmodified at the end except for its selection
     * masks.
     */
    virtual SharedEGraph transform(const SharedEGraph& graph) = 0;
    virtual TransformType type() const = 0;
    virtual QString name() const = 0;
    virtual std::string cliName() const = 0;
};

}

Q_DECLARE_INTERFACE(Elve::TransformPlugin,TransformPlugin_iid)

///ELVE_TRANSFORM helps to define base properties of a TRANSFORM
#define ELVE_TRANSFORM(Transform,aname,acliname,atype) \
    public: \
        inline Elve::TransformType type() const override {return (atype);} \
        inline QString name() const override {return (aname);} \
        inline std::string cliName() const override {return (acliname);}

#endif // GRAPHTRANSFORM_H
