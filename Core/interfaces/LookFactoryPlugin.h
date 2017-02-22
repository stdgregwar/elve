#ifndef LOOKFACTORYPLUGIN_H
#define LOOKFACTORYPLUGIN_H

#include <QObject>

#include "Plugin.h"
#include "EdgeLook.h"

#define LookFactoryPlugin_iid "ch.epfl.lap.elfe.LookFactoryPugin"
class LookFactoryPlugin;

typedef std::shared_ptr<LookFactoryPlugin> SharedLook;

class LookFactoryPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    LookFactoryPlugin();
    LookFactoryPlugin(const LookFactoryPlugin& other);

    virtual QString lookName() const = 0;
    virtual std::string cliName() const = 0;
    virtual SharedLook create() = 0;

    virtual NodeLook* node(const Node& n) = 0;
    virtual EdgeLook* edge(const NodeLook& ancestor, const NodeLook& children) = 0;

    NodeLook* getNode(const Node& n);
    EdgeLook* getEdge(const NodeLook& ancestor, const NodeLook& children);
    virtual OrientationHint orientationHint() const;
    void updateTransforms();
private:
    QTransform mOrientationTransform;
    QTransform mInverseOrientationTransform;
};

Q_DECLARE_INTERFACE(LookFactoryPlugin,LookFactoryPlugin_iid)

#define ELVE_LOOK(Look,full_name,cli_name)\
 public:\
    Look();\
    inline SharedLook create() override {return std::make_shared<Look>(*this);}\
    inline QString lookName() const override {return (full_name);}\
    inline std::string cliName() const override {return (cli_name);}

#endif // LOOKFACTORYPLUGIN_H
