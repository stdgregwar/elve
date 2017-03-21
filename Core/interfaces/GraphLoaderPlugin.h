#ifndef LOADERINTERFACE_H
#define LOADERINTERFACE_H


#include <QtPlugin>
#include <QString>
#include "../Graph.h"
#include "Plugin.h"

#define GraphLoaderPlugin_iid "ch.epfl.lap.elfe.GraphLoaderPlugin"

namespace Elve {

/**
 * @brief The GraphLoaderPlugin abstract class
 *
 * GraphLoaderPlugin is a QPlugin interface that allow to extend the range of file
 * format that elve can import.
 */
class GraphLoaderPlugin : public Plugin
{
    public:
        virtual ~GraphLoaderPlugin(){}

        /**
         * @brief load the graph from the given file
         * @param filepath path of the file
         * @return A valid SharedGraph
         *
         * Re-implement this method in your plugin.
         *
         * Here should be placed the import logic
         */
        virtual SharedGraph load(const QString& filepath) = 0;

        /**
         * @brief tells the file filter
         * @return
         *
         * This is the Qt filefilter that state the valid extensions and files to
         * import
         */
        virtual QString fileFilter() = 0;

        /**
         * @brief tells the name of the import format
         * @return the name
         *
         * The format name is the name diplayed in the Gui menus and captions,
         * the true name of the importer.
         */
        virtual QString formatName() = 0;

        /**
         * @brief command line interface name
         * @return
         *
         * The name of the plugin in the command line, should not contain any spaces
         * and be as short as possible
         */
        virtual std::string cliName() = 0;
};

}

Q_DECLARE_INTERFACE(Elve::GraphLoaderPlugin,GraphLoaderPlugin_iid)

#define ELVE_LOADER(Loader,full_name,filter,cli_name)\
 public:\
    inline QString formatName() override {return (full_name);}\
    inline QString fileFilter() override {return filter;}\
    inline std::string cliName() override {return (cli_name);}
#endif // LOADERINTERFACE_H
