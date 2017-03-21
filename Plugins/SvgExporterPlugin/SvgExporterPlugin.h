#ifndef SVGEXPORTERPLUGIN_H
#define SVGEXPORTERPLUGIN_H

#include <interfaces/FileExporterPlugin.h>

class SvgExporterPlugin : public Elve::FileExporterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.svgexporter")
    Q_INTERFACES(Elve::FileExporterPlugin)
public:
    ELVE_EXPORTER(SvgExporterPlugin,"SVG","SVG Files (*.svg)","svg")
    SvgExporterPlugin();
    void exportGraph(const QString& filename, const Elve::SharedEGraph& eg) override;
};

#endif // SVGEXPORTERPLUGIN_H
