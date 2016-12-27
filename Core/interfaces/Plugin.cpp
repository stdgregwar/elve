#include "Plugin.h"

Plugin::Plugin() : mOpts("plugin options")
{

}

void Plugin::configPath(const std::string& path)
{
    mConfigPath = path;
}

const std::string& Plugin::configPath() const
{
    return mConfigPath;
}

po::options_description& Plugin::opts() {
    return mOpts;
}

po::positional_options_description& Plugin::pods()
{
    return mPods;
}
