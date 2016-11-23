#ifndef SIMPLELAYOUTPLUGIN_H
#define SIMPLELAYOUTPLUGIN_H

#include <LayoutPlugin.h>

class SimpleLayout : public LayoutPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.simplelayout")
    Q_INTERFACES(LayoutPlugin)

    ELVE_LAYOUT(SimpleLayout,"Simple-Force","simple")
    virtual void setGraph(SharedGraph g) override;
    virtual void tick(float dt, bool fast) override;
};

#endif // LEVELLAYOUTPLUGIN_H
