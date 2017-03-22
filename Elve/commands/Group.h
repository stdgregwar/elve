#pragma once

#include <alice/command.hpp>

namespace Elve {

using namespace alice;

class GroupCommand : public command
{
public:
    GroupCommand(const environment::ptr& env) : command(env,"Group"),name("group") {
        pod.add("mask",1).add("name",1);

        opts.add_options()
                ("mask,m",po::value(&mask)->default_value(0),"Selection mask index")
                ("name,n",po::value(&name)->default_value(name),"Name of the group")
                ("index,i",po::value(&sid));
    }

    bool execute() override {
        SharedEGraph eg = env->store<SharedEGraph>().current();
        SharedEGraph grouped;
        if(is_set("mask")) {
            grouped =  eg->group(eg->selection(mask),name);
        } else {
            grouped = eg->group(eg->currentSelection(),name);
        }
        env->store<SharedEGraph>().current() = grouped;
        if(eg->view()) eg->view()->setGraph(grouped,0);
        return true;
    }
private:
    NodeName name;
    int sid;
    int mask;
};

}
