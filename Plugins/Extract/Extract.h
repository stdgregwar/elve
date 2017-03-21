#pragma once

#include <interfaces/GraphTransformPlugin.h>

class Extract : public Elve::GraphTransformPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.Extract")
    Q_INTERFACES(Elve::GraphTransformPlugin)
public:
    ELVE_TRANSFORM(Extract,"Extract selection","extract",Elve::OTHER)
    Extract();
    Elve::SharedEGraph transform(const Elve::SharedEGraph& eg) override;
private:
    int mMask;
};
