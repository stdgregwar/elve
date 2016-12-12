#ifndef PLUGIN_H
#define PLUGIN_H

#include <boost/program_options.hpp>

namespace po = boost::program_options; //Find more elegant way

class Plugin
{
public:
    using rule_t  = std::pair<std::function<bool()>, std::string>;
    using rules_t = std::vector<rule_t>;

    Plugin();
    po::options_description& opts();
    po::positional_options_description& pods();
private:
    po::options_description mOpts;
    po::positional_options_description mPods;
};


#endif // PLUGIN_H
