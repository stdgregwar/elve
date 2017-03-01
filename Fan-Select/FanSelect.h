#pragma once

#include <interfaces/GraphTransformPlugin.h>

class FanSelect : public Elve::GraphTransformPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.FanSelectTransform")
    Q_INTERFACES(Elve::GraphTransformPlugin)
public:
    ELVE_TRANSFORM(FanSelect,"Select Fans","fanselect",Elve::SELECTION)
    FanSelect();
    Elve::SharedEGraph transform(const Elve::SharedEGraph& eg) override;
private:
    int mInMask;
    int mOutMask;
};
