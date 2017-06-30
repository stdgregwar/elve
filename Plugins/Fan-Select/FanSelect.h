#pragma once

#include <interfaces/TransformPlugin.h>

class FanSelect : public Elve::TransformPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.FanSelectTransform")
    Q_INTERFACES(Elve::TransformPlugin)
public:
    ELVE_TRANSFORM(FanSelect,"Select Fans","fanselect",Elve::SELECTION)
    FanSelect();
    Elve::SharedEGraph transform(const Elve::SharedEGraph& eg) override;
private:
    int mInMask;
    int mOutMask;
};
