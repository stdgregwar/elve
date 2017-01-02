#ifndef GRAPHTRANSFORM_H
#define GRAPHTRANSFORM_H

#include <EGraph.h>
#include "Plugin.h"

#define GraphTransformPlugin_iid "ch.epfl.lap.elfe.GraphTransformPlugin"

enum TransformType {
    SELECTION,
    OTHER
};

class GraphTransformPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    virtual TransformType type() const = 0;
    virtual QString name() const = 0;
    virtual std::string cliName() const = 0;
    virtual SharedEGraph transform(const SharedEGraph& graph) = 0;
};

Q_DECLARE_INTERFACE(GraphTransformPlugin,GraphTransformPlugin_iid)

#define ELVE_TRANSFORM(Transform,aname,acliname,atype) \
    public: \
        inline TransformType type() const override {return (atype);} \
        inline QString name() const override {return (aname);} \
        inline std::string cliName() const override {return (acliname);}

#endif // GRAPHTRANSFORM_H
