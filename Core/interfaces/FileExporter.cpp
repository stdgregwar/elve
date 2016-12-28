#include "FileExporter.h"
#include "GraphWidget.h"

FileExporter::FileExporter()
{
}

QGraphicsScene* FileExporter::sceneForGraph(const SharedEGraph& eg) {
    if(eg->view()) {
        return eg->view()->scene();
    } else {
        if(eg->layout()) {
            GraphWidget* gw = new GraphWidget();
            gw->setGraph(eg);
            return eg->view()->scene();
        } else {
            return nullptr;
        }
    }
}
