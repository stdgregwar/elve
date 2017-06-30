#pragma once

#include "Plugin.h"
#include <interfaces/LoaderPlugin.h>
#include <PluginManager.h>

namespace Elve {

using namespace alice;

//Loader command
class LoaderCommand : public PluginCommand
{
public:
    LoaderCommand(LoaderPlugin* pl, const environment::ptr& env) : PluginCommand(pl,env, (pl->formatName() + " Loader").toStdString()),
      mLoader(pl)
    {
        pod.add("filename",1);
        opts.add_options()
                ("filename,f",po::value(&mFilename),
                 ("filename of the " + pl->formatName() + " file to load").toStdString().c_str());

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        return  {
            file_exists_if_set( *this, process_filename( mFilename ),"filename")
        };
    }

    bool execute() override {
        auto& graphs = env->store<SharedEGraph>();
        SharedEGraph eg = std::make_shared<ExtendedGraph>(mLoader->load(QString::fromStdString(mFilename)));
        eg->setLook(PluginManager::get().defaultLook());
        eg->setLayout(PluginManager::get().defaultLayout());
        graphs.push(eg);
        return true;
    }
private:
    std::string mFilename;
    LoaderPlugin* mLoader;
};

}
