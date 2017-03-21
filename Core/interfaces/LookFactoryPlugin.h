#ifndef LOOKFACTORYPLUGIN_H
#define LOOKFACTORYPLUGIN_H

#include <QObject>

#include "Plugin.h"
#include "EdgeLook.h"

#define LookFactoryPlugin_iid "ch.epfl.lap.elfe.LookFactoryPugin"

namespace Elve {

class LookFactoryPlugin;

typedef std::shared_ptr<LookFactoryPlugin> SharedLook;

/**
 * @brief The LookFactoryPlugin abstract class
 *
 * This QPlugin interface allow to replace the way the graph's node are drawn in
 * the viewport. This class use the abstract factory pattern. Meaning that when
 * re-implementing it's sole role will be to instantiate subclasses of abstract
 * objects.
 *
 * -NodeLooks for the nodes
 * -EdgeLooks for the edges
 */
class LookFactoryPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    /**
     * @brief LookFactoryPlugin
     */
    LookFactoryPlugin();

    /**
     * @brief Copy constructor to conserve option when replicating the factory
     * @param other another factory
     */
    LookFactoryPlugin(const LookFactoryPlugin& other);

    /**
     * @brief name of the look as displayed in the GUI
     * @return
     */
    virtual QString lookName() const = 0;

    /**
     * @brief name of the look as used in the command line interface
     * @return
     */
    virtual std::string cliName() const = 0;

    /**
     * @brief create a shared copy of this Look
     * @return
     */
    virtual SharedLook create() = 0;

    /**
     * @brief node producing method
     * @param n a node to create look for
     * @return a subclass of NodeLook
     *
     * Re-implement this method.
     *
     * In this method you're given a node and must output a subclass of NodeLook
     * representing the node in a QGraphicsScene.
     */
    virtual NodeLook* node(const Node& n) = 0;

    /**
     * @brief edge producing method
     * @param ancestor nodelook from which the edge comes
     * @param children nodelook to which the edge goes
     * @return a subclass of EdgeLook
     *
     * Re-implement this method
     *
     * In this method you're given two NodeLook that you previously returned
     * from the node(const Node& n) method and must return an subclass of EdgeLook
     * that will represent the edge between those nodes.
     */
    virtual EdgeLook* edge(const NodeLook& ancestor, const NodeLook& children) = 0;

    /**
     * @brief getNode impl detail
     * @param n
     * @return
     */
    NodeLook* getNode(const Node& n);

    /**
     * @brief getEdge impl detail
     * @param ancestor
     * @param children
     * @return
     */
    EdgeLook* getEdge(const NodeLook& ancestor, const NodeLook& children);

    /**
     * @brief reimplement this to tell the layout you're in another orientation
     * that BottomUp
     * @return
     */
    virtual OrientationHint orientationHint() const;
};

}

Q_DECLARE_INTERFACE(Elve::LookFactoryPlugin,LookFactoryPlugin_iid)

///ELVE_LOOK helps define the main lookfactory properties
#define ELVE_LOOK(Look,full_name,cli_name)\
 public:\
    Look();\
    inline Elve::SharedLook create() override {return std::make_shared<Look>(*this);}\
    inline QString lookName() const override {return (full_name);}\
    inline std::string cliName() const override {return (cli_name);}

#endif // LOOKFACTORYPLUGIN_H
