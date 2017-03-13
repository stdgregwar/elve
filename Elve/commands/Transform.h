#pragma once

#include "Plugin.h"
#include <ExtendedGraph.h>
#include <GraphWidget.h>
#include <MainWindow.h>

namespace Elve {

using namespace alice;

class TransformCommand : public PluginCommand
{
public:
    TransformCommand(GraphTransformPlugin* pl, const environment::ptr& env) : PluginCommand(pl,env, (pl->name()).toStdString()),
      mTrans(pl)
    {
        opts.add_options()
                ("input,i",po::value(&mInput),
                 "index of the graph to transform (default : current)")
                ("output,o",po::value(&mOutput),"output index for the graph (opt)")
                ("newtab,n","open in a new tab");

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        return  {
            //file_exists_if_set( *this, process_filename( mFilename ),"filename")
        };
    }

    bool execute() override {
        SharedEGraph input;
        if(is_set("input")) {
            input = env->store<SharedEGraph>()[mInput];
        } else {
            input = env->store<SharedEGraph>().current();
        }
        SharedEGraph output = mTrans->transform(input);
        if(is_set("output")) {
            env->store<SharedEGraph>()[mOutput] = output;
        } else {
            if(mTrans->type() != SELECTION) env->store<SharedEGraph>().extend();
            env->store<SharedEGraph>().current() = output;
        }
        env->store<SharedEGraph>().notify();
        if(is_set("newtab")) {
            MainWindow::get().newWindowWithFile(output,output->graph()->filename());
        } else {
            output->view()->setGraph(output);
        }
        if(output->view() && mTrans->type() == SELECTION) {
            output->view()->updateSelectionColor();
        }
        return true;
    }
private:
    GraphTransformPlugin* mTrans;
    int mInput;
    int mOutput;
};

}
