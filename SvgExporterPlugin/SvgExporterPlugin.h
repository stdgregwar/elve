#ifndef SVGEXPORTERPLUGIN_H
#define SVGEXPORTERPLUGIN_H

#include <interfaces/FileExporter.h>

class SvgExporterPlugin : public FileExporter
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.svgexporter")
    Q_INTERFACES(FileExporter)
public:
    ELVE_EXPORTER(SvgExporterPlugin,"SVG","SVG Files (*.svg)","svg")
    SvgExporterPlugin();
    void exportGraph(const QString& filename, const SharedEGraph& eg) override;
};

#endif // SVGEXPORTERPLUGIN_H
