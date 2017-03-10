#ifndef FILEEXPORTER_H
#define FILEEXPORTER_H

#include <QGenericPlugin>
#include "Plugin.h"
#include <ExtendedGraph.h>

#define ExporterPlugin_iid "ch.epfl.lap.elfe.LayoutPlugin"

namespace Elve {

class FileExporterPlugin : public QObject, public Plugin
{
    Q_OBJECT
public:
    FileExporterPlugin();
    virtual void exportGraph(const QString& filename, const SharedEGraph& eg) = 0;
    QGraphicsScene* sceneForGraph(const SharedEGraph& eg);
    virtual QString formatName() = 0;
    virtual QString fileFilter() = 0;
    virtual std::string cliName() = 0;
};

}
Q_DECLARE_INTERFACE(Elve::FileExporterPlugin,ExporterPlugin_iid)

#define ELVE_EXPORTER(Exporter,full_name,filter,cli_name)\
 public:\
    inline QString formatName() override {return (full_name);}\
    inline QString fileFilter() override {return filter;}\
    inline std::string cliName() override {return (cli_name);}

#endif // FILEEXPORTER_H
