#pragma once

#include <alice/command.hpp>
#include <interfaces/Plugin.h>

namespace Elve {

using namespace alice;

class PluginCommand : public command
{
public : PluginCommand(Plugin* pl,const environment::ptr& env,const std::string& name) : command(env,name)
    {
        pl->set_vmap(&vm);
    }
};

}
