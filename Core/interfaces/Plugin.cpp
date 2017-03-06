#include "Plugin.h"

namespace Elve {
Plugin::Plugin() : mOpts("plugin options"), mVMap(nullptr)
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

void Plugin::basePath(const QString& path) {
    mBasePath = path;
}

const QString& Plugin::basePath()const {
    return mBasePath;
}

po::options_description& Plugin::opts() {
    return mOpts;
}

void Plugin::set_vmap(const po::variables_map* map) {
    mVMap = map;
}

bool Plugin::is_set(const std::string& opt) const {
    return mVMap && mVMap->count(opt);
}

po::positional_options_description& Plugin::pods()
{
    return mPods;
}
}
