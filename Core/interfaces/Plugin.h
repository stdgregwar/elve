#ifndef PLUGIN_H
#define PLUGIN_H

#include <boost/program_options.hpp>
#include <string>
#include <QString>

namespace po = boost::program_options; //Find more elegant way

namespace Elve {

/**
 * @brief base of all plugin interface in elve
 *
 * This class is mainly an helper aimed at offering basic options and functionnality
 * to every plugin in elve.
 */
class Plugin
{
public:
    /**
     * @brief Plugin
     */
    Plugin();

    /**
     * @brief get the boost::program_option ctor
     * @return
     *
     * Use this to add option to the plugin you're writing, they will be
     * immediatly available as option in the CLI
     */
    po::options_description& opts();

    /**
     * @brief get the positional options ctor
     * @return
     *
     * Use this to add positionnal options as in Plugin::opts()
     */
    po::positional_options_description& pods();

    /**
     * @brief set the path to the configuration ".ini" file of the plugin
     * @param path the path
     */
    void configPath(const std::string& path);

    /**
     * @brief set the path from the excecutable to the folder the plugin is in
     * @param path the path
     */
    void basePath(const QString &path);

    /**
     * @brief get the path from the excecutable to the folder the plugin is in
     * @return
     */
    const QString& basePath()const;

    /**
     * @brief get the path to the configuration ".ini" file of the plugin
     * @return
     */
    const std::string& configPath() const;

    /**
     * @brief used internally to let option variables be known to the plugin
     * @param map
     */
    void set_vmap(const po::variables_map* map);

    /**
     * @brief test wheter a particular option variable is set
     * @param opt the option name
     * @return
     */
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
