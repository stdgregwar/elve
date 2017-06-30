#pragma once

#include <alice/command.hpp>
#include <MainWindow.h>

namespace Elve {

using namespace alice;

class ClearAllCommand : public command
{
public:
    ClearAllCommand(const environment::ptr& env) : command(env,"Reset all ELVE, save your progress before!") {

    }
    bool execute() override {
        env->store<SharedEGraph>().clear();
        MainWindow::get().closeAllTabs();
        return true;
    }
};

}
