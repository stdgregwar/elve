#ifndef FILEEXPORTER_H
#define FILEEXPORTER_H

#include <QGenericPlugin>
#include "Plugin.h"
#include <ExtendedGraph.h>

#define ExporterPlugin_iid "ch.epfl.lap.elfe.LayoutPlugin"

namespace Elve {

/**
 * @brief The FileExporterPlugin abstract class
 *
 * The FileExporterPlugin is a QPlugin interface allowing to create file exporter
 * plugins. These plugins can be dynamically loaded at runtime to extends the
 * range of files elve can export.
 */
class FileExporterPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    /**
     * @brief FileExporterPlugin
     */
    FileExporterPlugin();

    /**
     * @brief export the given graph
     * @param filename name of the file to write to
     * @param eg graph to export
     *
     * Must be reimplemented in plugins, contains the export logic.
     */
    virtual void exportGraph(const QString& filename, const SharedEGraph& eg) = 0;

    /**
     * @brief get the graph scene, ensuring that it is created
     * @param eg
     * @return a valid QGraphicsScene, nullptr if something went wrong
     */
    QGraphicsScene* sceneForGraph(const SharedEGraph& eg);

    /**
     * @brief tells the name of the export format
     * @return the name
     *
     * The format name is the name diplayed in the Gui menus and captions,
     * the true name of the exporter.
     */
    virtual QString formatName() = 0;

    /**
     * @brief tells the filefilter
     * @return
     *
     * This is the Qt filefilter that state the valid extensions and files to
     * export
     */
    virtual QString fileFilter() = 0;

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
Q_DECLARE_INTERFACE(Elve::FileExporterPlugin,ExporterPlugin_iid)

///ELVE EXPORTER macro is used to quickly state the various parameters of the
/// exporter
#define ELVE_EXPORTER(Exporter,full_name,filter,cli_name)\
 public:\
    inline QString formatName() override {return (full_name);}\
    inline QString fileFilter() override {return filter;}\
    inline std::string cliName() override {return (cli_name);}

#endif // FILEEXPORTER_H
