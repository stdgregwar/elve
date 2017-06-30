#pragma once

#include <interfaces/TransformPlugin.h>

class Extract : public Elve::TransformPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.Extract")
    Q_INTERFACES(Elve::TransformPlugin)
public:
    ELVE_TRANSFORM(Extract,"Extract selection","extract",Elve::OTHER)
    Extract();
    Elve::SharedEGraph transform(const Elve::SharedEGraph& eg) override;
private:
    int mMask;
};
