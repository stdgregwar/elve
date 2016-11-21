#ifndef LEVELLAYOUTPLUGIN_H
#define LEVELLAYOUTPLUGIN_H

#include <QGenericPlugin>
#include <LayoutPlugin.h>
#include <System.h>

class SimpleLayout : public LayoutPlugin
{
public:
    SimpleLayout();
    //virtual void setGraph(SharedGraph graph);
    virtual void setGraph(SharedGraph g) override;
    virtual void tick(float dt, bool fast) override;
    virtual QString layoutName() override;
private:
};

ELVE_EXPORT_LAYOUT(SimpleLayout, "Simple-Force","simple")

#endif // LEVELLAYOUTPLUGIN_H
