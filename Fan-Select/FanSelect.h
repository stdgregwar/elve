#pragma once

#include <interfaces/GraphTransformPlugin.h>

class FanSelect : public GraphTransformPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.FanSelectTransform")
    Q_INTERFACES(GraphTransformPlugin)
public:
    ELVE_TRANSFORM(FanSelect,"Select Fans","fanselect",SELECTION)
    FanSelect();
    SharedEGraph transform(const SharedEGraph& eg) override;
private:
    int mInMask;
    int mOutMask;
};
