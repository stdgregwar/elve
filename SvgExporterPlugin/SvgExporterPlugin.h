#ifndef SVGEXPORTERPLUGIN_H
#define SVGEXPORTERPLUGIN_H

#include <interfaces/FileExporterPlugin.h>

class SvgExporterPlugin : public FileExporterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.svgexporter")
    Q_INTERFACES(FileExporterPlugin)
public:
    ELVE_EXPORTER(SvgExporterPlugin,"SVG","SVG Files (*.svg)","svg")
    SvgExporterPlugin();
    void exportGraph(const QString& filename, const SharedEGraph& eg) override;
};

#endif // SVGEXPORTERPLUGIN_H
