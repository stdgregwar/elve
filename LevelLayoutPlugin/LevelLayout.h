#ifndef LEVELLAYOUTPLUGIN_H
#define LEVELLAYOUTPLUGIN_H

#include <LayoutPlugin.h>

class LevelLayout : public LayoutPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.levelLayout")
    Q_INTERFACES(LayoutPlugin)

    ELVE_LAYOUT(LevelLayout,"Level-Force","level")
    LevelLayout();
    virtual void setGraph(SharedGraph g) override;
    virtual void tick(float dt, bool fast) override;
private:
    qreal mK = 2; //Plugin options with default values
    qreal mL0 = 0;
    qreal mDamp = 2;
    qreal mMinUnit = 256.0;
    qreal mMinIOUnit = 128.0;
};

#endif // LEVELLAYOUTPLUGIN_H
