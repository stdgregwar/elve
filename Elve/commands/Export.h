#pragma once

#include "Plugin.h"
#include <interfaces/FileExporterPlugin.h>

namespace Elve {

using namespace alice;

class ExportCommand : public PluginCommand
{
public:
    ExportCommand(FileExporterPlugin* pl, const environment::ptr& env) : PluginCommand(pl,env, (pl->formatName() + " Saver").toStdString()),
      mSaver(pl)
    {
        pod.add("filename",1);
        opts.add_options()
                ("filename,f",po::value(&mFilename),
                 ("filename of the " + pl->formatName() + " file to save").toStdString().c_str());

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        return  {
            //file_exists_if_set( *this, process_filename( mFilename ),"filename")
        };
    }

    bool execute() override {
        auto& graph = env->store<SharedEGraph>().current();
        //Todo make sure scene is available, or find better way
        mSaver->exportGraph(QString::fromStdString(mFilename),graph);
        return true;
    }

private:
    std::string mFilename;
    FileExporterPlugin* mSaver;
};

}
