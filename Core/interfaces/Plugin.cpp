#include "Plugin.h"

Plugin::Plugin()
{

}

po::options_description& Plugin::opts() {
    return mOpts;
}

po::positional_options_description& Plugin::pods()
{
    return mPods;
}
