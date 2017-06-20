#include "Exporter.h"
#include <GraphWidget.h>
#include <QtSvg/QSvgGenerator>
#include <QDebug>

using namespace Elve;

ExporterName::ExporterName()
{
    int myOp;
    opts().add_options() //Use boost options to setup plugin command options, see boost_program_option doc
            ("myoption",po::value(&myOp),"an integer option you can set while calling cmd");
}

void ExporterName::exportGraph(const QString& filename, const SharedEGraph& eg) {
    //HERE you are free to write the graph to given filename in any way you want
    //Note that you could even pass arbitrary string as filename from CLI and do wathever you want, including sending graph to another software for example
}
