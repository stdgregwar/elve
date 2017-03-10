#ifndef LAYOUTPLUGIN_H
#define LAYOUTPLUGIN_H

#include <unordered_map>
#include <Graph.h>
#include <QVector2D>
#include <QtPlugin>

#include <System.h>

#include "Plugin.h"

#define LayoutPlugin_iid "ch.epfl.lap.elfe.LayoutPlugin"

namespace Elve {

class LayoutPlugin;
typedef std::shared_ptr<LayoutPlugin> SharedLayout;
class ExtendedGraph;
typedef std::shared_ptr<ExtendedGraph> SharedEGraph;

class LayoutPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    LayoutPlugin();
    LayoutPlugin(const LayoutPlugin& other);
    void clear();
    virtual void setGraph(SharedGraph graph) = 0;
    QVector2D startPosition(const NodeID& id);
    void setGraph(SharedEGraph g, const NodePositions& positions);
    virtual void quickSim(size_t ticks);
    virtual void tick(float dt, bool fast);
    virtual SharedLayout create() = 0;
    virtual QString name() = 0;
    virtual std::string cliName() = 0;
    System& system();
    const System& system() const;
    virtual ~LayoutPlugin(){}
private:
    System mSystem;
    NodePositions mStartPositions;
};

}

Q_DECLARE_INTERFACE(Elve::LayoutPlugin,LayoutPlugin_iid)

#define ELVE_LAYOUT(Layout,full_name,cli_name)\
 public:\
    inline Elve::SharedLayout create() override {return std::make_shared<Layout>(*this);}\
    inline QString name() override {return (full_name);}\
    inline std::string cliName() override {return (cli_name);}

#endif // LAYOUTPLUGIN_H
