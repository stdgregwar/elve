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
class GraphWidgetListener;
class LayoutPlugin;
typedef std::shared_ptr<LayoutPlugin> SharedLayout;
class ExtendedGraph;
typedef std::shared_ptr<ExtendedGraph> SharedEGraph;

/**
 * @brief The LayoutPlugin abstract class
 *
 * This QPlugin interface allow to add new layout types and systems to elve. It
 * redefines how the nodes are positioned and animated.
 *
 * As Graph nodes don't have a position on their own, the Layouts output a
 * collection of Points that are then linked with movable objects in the
 * viewport.
 */
class LayoutPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    /**
     * @brief LayoutPlugin
     */
    LayoutPlugin();

    /**
     * @brief copy constructor to inherit option values
     * @param other
     */
    LayoutPlugin(const LayoutPlugin& other);

    /**
     * @brief clear, called when re-using the layout object for another graph
     *
     * \deprecated{This was used at a point where elve had only one viewport.}
     */
    void clear();

    /**
     * @brief set the graph that this layout must animate
     * @param graph A graph to genereate layout for
     */
    virtual void setGraph(SharedGraph graph) = 0;

    /**
     * @brief startPosition of a Node
     * @param id the node id
     * @return Either the previous position or a random one
     *
     * This is used to abstract the place of the layout in the pipeline, by calling
     * this method to get starting position of a particular node, one ensure that
     * if the layout is succeding another in the viewport, simulation will continue
     * from the same state as before but with potentially new constraints.
     */
    QVector2D startPosition(const NodeID& id);

    /**
     * @brief alternate setGraph where you can provide initial position
     * @param g the graph
     * @param positions a map of nodes positions
     *
     * This is the entry point of elve's core to the plugin, the default
     * implementation of this method calls the other pure virtual method after
     * optionnaly retrieving the ancient positions
     */
    virtual void setGraph(SharedEGraph g, const NodePositions& positions);

    /**
     * @brief tells the plugin to simulate a certain number of ticks in a quick way
     * @param ticks the tick count
     *
     * Default implementation of this method just call tick repeatedly
     */
    virtual void quickSim(size_t ticks);

    /**
     * @brief advance the simulation one step
     * @param dt time delta in seconds since last invoke
     * @param fast indicate that the simulation do not need to acknowledge the
     * gui part.
     *
     * The default implementation of this method ticks the default physical system.
     */
    virtual void tick(float dt, bool fast);

    /**
     * @brief make a shared copy of this layout
     * @return
     */
    virtual SharedLayout create() = 0;

    /**
     * @brief the main name of the layout displayed in the GUIs
     * @return
     */
    virtual QString name() = 0;

    /**
     * @brief the command line interface name of the GUI
     * @return should not contain spaces
     */
    virtual std::string cliName() = 0;

    /**
     * @brief called when the command is started from the GUI
     * @param listener the command listener currently bound
     * @param graph the graph
     *
     * Usefull if you want to display custom GUI wizard or dialogs when your plugin
     * is triggered, You can do anything in this method but don't forget that it
     * is executed as part of the Qt event loop and will stall everything until
     * it returns.
     */
    virtual void uiStart(Elve::GraphWidgetListener* listener,const SharedEGraph& graph);

    /**
     * @brief get the default physical system
     * @return
     */
    System& system();

    /**
     * @brief get a const reference to the default physical system
     * @return
     */
    const System& system() const;

    /**
     * @brief ~LayoutPlugin virtual for inheritance
     */
    virtual ~LayoutPlugin(){}
private:
    System mSystem;
    NodePositions mStartPositions;
};

}

Q_DECLARE_INTERFACE(Elve::LayoutPlugin,LayoutPlugin_iid)

///ELVE_LAYOUT is an helper to declare the main parameters of the layout
#define ELVE_LAYOUT(Layout,full_name,cli_name)\
 public:\
    inline Elve::SharedLayout create() override {return std::make_shared<Layout>(*this);}\
    inline QString name() override {return (full_name);}\
    inline std::string cliName() override {return (cli_name);}

#endif // LAYOUTPLUGIN_H
