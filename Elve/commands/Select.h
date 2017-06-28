#pragma once

#include <GraphWidget.h>
#include <alice/command.hpp>

namespace Elve {

using namespace alice;

class SelectCommand : public command
{
public:
    SelectCommand(const environment::ptr& env) : command(env,"Controls selection masks") {
        pod.add("mask",1).add("nodeIDs",-1);

        opts.add_options()
                ("mask,m",po::value(&mask)->default_value(0),"Selection mask index")
                ("index,i",po::value(&sid))
                ("clear,c","clear existing selection")
                ("all,a","select all")
                ("add","add nodes")
                ("sub","sub nodes")
                ("nodeIDs",po::value(&ids),"node ids to process");
    }
    bool execute() override {
        SharedEGraph eg = env->store<SharedEGraph>().current();
        Selection& s = eg->selection(mask);
        if(is_set("clear")) {
            s.clear();
        }
        if(is_set("all")) {
            for(const auto& p : eg->graph()->nodes()) {
                s.add(p.first);
            }
        }
        auto op = static_cast<void(Selection::*)(const NodeID&)>(&Selection::add);
        if(is_set("sub")) {
            op = static_cast<void(Selection::*)(const NodeID&)>(&Selection::sub);
        }
        for(const NodeID& id : ids) {
            (s.*op)(id);
        }
        if(eg->view()) {
            eg->view()->updateSelectionColor();
            MainWindow::get().selectionChanged(eg);
        }
        ids.clear();
        return true;
    }
private:
    int sid;
    NodeIDs ids;
    int mask;
};

}
