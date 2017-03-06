#ifndef LOADERINTERFACE_H
#define LOADERINTERFACE_H


#include <QtPlugin>
#include <QString>
#include "../Graph.h"
#include "Plugin.h"

#define GraphLoaderPlugin_iid "ch.epfl.lap.elfe.GraphLoaderPlugin"

namespace Elve {

class GraphLoaderPlugin : public Plugin
{
    public:
        virtual ~GraphLoaderPlugin(){}
        virtual SharedGraph load(const QString& filepath) = 0;
        virtual QString fileFilter() = 0;
        virtual QString formatName() = 0;
        virtual std::string cliName() = 0;
};

}

Q_DECLARE_INTERFACE(Elve::GraphLoaderPlugin,GraphLoaderPlugin_iid)

#define ELVE_LOADER(Loader,full_name,filter,cli_name)\
 public:\
    inline QString formatName() override {return (full_name);}\
    inline QString fileFilter() override {return filter;}\
    inline std::string cliName() override {return (cli_name);}
#endif // LOADERINTERFACE_H
