#ifndef LEVELLAYOUTPLUGIN_H
#define LEVELLAYOUTPLUGIN_H

#include <QGenericPlugin>
#include <LayoutPlugin.h>
#include <System.h>

class LevelLayout : public LayoutPlugin
{
public:
    LevelLayout();
    //virtual void setGraph(SharedGraph graph);
    virtual void setGraph(SharedGraph g) override;
    virtual void tick(float dt, bool fast) override;
    virtual QString layoutName() override;
private:
};

ELVE_EXPORT_LAYOUT(LevelLayout,"Level-Layout","level")

#endif // LEVELLAYOUTPLUGIN_H
