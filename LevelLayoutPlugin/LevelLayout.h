#ifndef LEVELLAYOUTPLUGIN_H
#define LEVELLAYOUTPLUGIN_H

#include <QGenericPlugin>
#include <LayoutPlugin.h>
#include <System.h>

class LevelLayout : public QObject, public LayoutPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.LevelLayout")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(LayoutPlugin)
public:
    LevelLayout(QObject *parent = 0);
    //virtual void setGraph(SharedGraph graph);
    virtual void setGraph(SharedGraph g) override;
    virtual void tick(float dt, bool fast) override;
    virtual QString layoutName() override;
private:
};

#endif // LEVELLAYOUTPLUGIN_H
