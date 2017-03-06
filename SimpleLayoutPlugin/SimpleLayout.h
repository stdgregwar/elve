#ifndef SIMPLELAYOUTPLUGIN_H
#define SIMPLELAYOUTPLUGIN_H

#include <LayoutPlugin.h>

class SimpleLayout : public Elve::LayoutPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.simplelayout")
    Q_INTERFACES(Elve::LayoutPlugin)

    ELVE_LAYOUT(SimpleLayout,"Simple-Force","simple")
    SimpleLayout();
    virtual void setGraph(Elve::SharedGraph g) override;
    virtual void tick(float dt, bool fast) override;
private:
    qreal mK = 2; //Plugin options with default values
    qreal mL0 = 0;
    qreal mDamp = 4;
    qreal mMinUnit = 128.0;
    qreal mMinIOUnit = 128.0;
};

#endif // LEVELLAYOUTPLUGIN_H
