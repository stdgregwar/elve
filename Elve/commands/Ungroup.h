#pragma once

#include <alice/command.hpp>

namespace Elve {

using namespace alice;

class UngroupCommand : public command
{
public:
    UngroupCommand(const environment::ptr& env) : command(env,"Ungroup") {
        pod.add("mask",1).add("name",1);

        opts.add_options()
                ("mask,m",po::value(&mask)->default_value(0),"Selection mask index")
                ("index,i",po::value(&sid));
    }

    bool execute() override {
        SharedEGraph eg = env->store<SharedEGraph>().current();
        Selection& s = eg->selection(mask);
        SharedEGraph ungrouped = eg->ungroup(s);
        env->store<SharedEGraph>().current() = ungrouped;
        if(eg->view()) eg->view()->setGraph(ungrouped);
        return true;
    }
private:
    int sid;
    int mask;
};

}
