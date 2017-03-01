#pragma once

#include "Plugin.h"
#include <interfaces/LookFactoryPlugin.h>

namespace Elve {

using namespace alice;

class LookCommand : public PluginCommand
{
public:
    LookCommand(LookFactoryPlugin* pl, const environment::ptr& env) : PluginCommand(pl,env, (pl->lookName() + " Look").toStdString()),
      mLook(pl)
    {
        pod.add("index",1);
        opts.add_options()
                ("index",po::value(&id),
                 ("id of the graph to set look " + pl->lookName() + " on.").toStdString().c_str());

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        Store& graphs = env->store<SharedEGraph>();
        return {
            {[this,&graphs] {return (!is_set("id") && !graphs.empty()) || (id<graphs.size() && id > -1);},
                "if set id must be in store range, store must not be empty"}
        };
    }

    bool execute() override {
        auto& graphs = env->store<SharedEGraph>();
        SharedEGraph eg;
        if(is_set("index")){
            eg = graphs[id];
        } else {
            eg = graphs.current();
        }
        eg->setLook(mLook->create());
        return true;
    }
private:
    unsigned id = 0;
    LookFactoryPlugin* mLook;
};

}
