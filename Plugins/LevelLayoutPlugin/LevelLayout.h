#ifndef LEVELLAYOUTPLUGIN_H
#define LEVELLAYOUTPLUGIN_H

#include <LayoutPlugin.h>

class LevelLayout : public Elve::LayoutPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.levelLayout")
    Q_INTERFACES(Elve::LayoutPlugin)

    ELVE_LAYOUT(LevelLayout,"Level-Force","level")
    LevelLayout();
    void setGraph(Elve::SharedGraph g) override;
    void tick(float dt, bool fast) override;
    void uiStart(Elve::GraphWidgetListener* listener,const Elve::SharedEGraph& graph) override;
private:
    qreal mK = 4; //Plugin options with default values
    qreal mL0 = 0;
    qreal mDamp = 1;
    qreal mMinUnit = 128.0;
    qreal mMinIOUnit = 128.0;
    qreal mRepulsion = 75;
};


#endif // LEVELLAYOUTPLUGIN_H
