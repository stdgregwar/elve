#ifndef LOOKFACTORYPLUGIN_H
#define LOOKFACTORYPLUGIN_H

#include <QObject>

#include "Plugin.h"
#include "EdgeLook.h"

#define LookFactoryPlugin_iid "ch.epfl.lap.elfe.LookFactoryPugin"

class LookFactoryPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    LookFactoryPlugin();

    virtual QString lookName() const = 0;
    virtual std::string cliName() const = 0;

    virtual NodeLook* node(Node& n) = 0;
    virtual EdgeLook* edge(NodeLook& ancestor, NodeLook& children) = 0;
};

Q_DECLARE_INTERFACE(LookFactoryPlugin,LookFactoryPlugin_iid)

#define ELVE_LOOK(Factory,full_name,cli_name)\
 public:\
    inline QString lookName() override {return (full_name);}\
    inline std::string cliName() override {return (cli_name);}

#endif // LOOKFACTORYPLUGIN_H
