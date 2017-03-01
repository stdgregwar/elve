#include "FileExporterPlugin.h"
#include "GraphWidget.h"

namespace Elve {

FileExporterPlugin::FileExporterPlugin()
{
}

QGraphicsScene* FileExporterPlugin::sceneForGraph(const SharedEGraph& eg) {
    if(eg->view()) {
        return eg->view()->scene();
    } else {
        if(eg->layout()) {
            GraphWidget* gw = new GraphWidget(); //TODO manage memory
            gw->setGraph(eg);
            return eg->view()->scene();
        } else {
            return nullptr;
        }
    }
}

}
