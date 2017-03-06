#ifndef PLUGIN_H
#define PLUGIN_H

#include <boost/program_options.hpp>
#include <string>
#include <QString>

namespace po = boost::program_options; //Find more elegant way

namespace Elve {

class Plugin
{
public:
    Plugin();
    po::options_description& opts();
    po::positional_options_description& pods();
    void configPath(const std::string& path);
    void basePath(const QString &path);
    const QString& basePath()const;
    const std::string& configPath() const;
    void set_vmap(const po::variables_map* map);
    bool is_set(const std::string& opt) const;
private:
    std::string mConfigPath;
    QString mBasePath;
    po::options_description mOpts;
    po::positional_options_description mPods;
    const po::variables_map* mVMap;
};

}

#endif // PLUGIN_H
