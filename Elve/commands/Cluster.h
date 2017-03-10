#pragma once

#include <GraphWidget.h>

#include <alice/command.hpp>

namespace Elve {

using namespace alice;

class ClusterCommand : public command
{
public:
    ClusterCommand(const environment::ptr& env) : command(env,"cluster") {
        opts.add_options()("level,l",po::value(&level)->default_value(level),"clustering iterations count");
    }

    bool execute() override {
        SharedEGraph eg = env->store<SharedEGraph>().current();
        SharedGraph ng = eg->graph()->clusterize(level);
        SharedEGraph neg = eg->clone(ng,eg->positions());
        env->store<SharedEGraph>().current() = neg;
        if(neg->view()) neg->view()->setGraph(neg);
        return true;
    }
private:
    int level = 1;
};

}
