#include "SvgExporterPlugin.h"
#include <GraphWidget.h>
#include <QtSvg/QSvgGenerator>
#include <QDebug>

SvgExporterPlugin::SvgExporterPlugin()
{

}

void SvgExporterPlugin::exportGraph(const QString& filename, const SharedEGraph& eg) {
    QSvgGenerator svgGen;

    QGraphicsScene* scene = sceneForGraph(eg);
    if(!scene) {
        throw std::runtime_error("No scene found for graph, maybe there is no layout set.");
    }
    qDebug() << "saving scene with" << scene->items().count() << "items to svg";
    qDebug() << "bounds :" << scene->itemsBoundingRect().size().toSize();
    svgGen.setFileName(filename);
    svgGen.setResolution(1);
    svgGen.setSize(scene->itemsBoundingRect().size().toSize());
    svgGen.setViewBox(scene->itemsBoundingRect());
    svgGen.setTitle(QString("%1 exported by ELVE").arg(eg->graph()->filename()));
    svgGen.setDescription("layouted using elve");

    QPainter painter;
    painter.begin(&svgGen);
    scene->render(&painter);
    painter.end();
}
