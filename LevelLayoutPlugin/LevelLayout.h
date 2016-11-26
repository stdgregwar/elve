#ifndef LEVELLAYOUTPLUGIN_H
#define LEVELLAYOUTPLUGIN_H

#include <LayoutPlugin.h>

class LevelLayout : public LayoutPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.levelLayout")
    Q_INTERFACES(LayoutPlugin)

    ELVE_LAYOUT(LevelLayout,"Level-Force","level")
    virtual void setGraph(SharedGraph g) override;
    virtual void tick(float dt, bool fast) override;
};

#endif // LEVELLAYOUTPLUGIN_H
