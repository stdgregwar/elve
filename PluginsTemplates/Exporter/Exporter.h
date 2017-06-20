#pragma once

#include <interfaces/FileExporterPlugin.h>

class ExporterName : public Elve::FileExporterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.ExporterName")
    Q_INTERFACES(Elve::FileExporterPlugin)
public:
    ELVE_EXPORTER(ExporterName,"Exporter name in GUI","File format for extension filtering (.ext)","exporter_format_name_for_cli")
    ExporterName();
    void exportGraph(const QString& filename, const Elve::SharedEGraph& eg) override;
};
