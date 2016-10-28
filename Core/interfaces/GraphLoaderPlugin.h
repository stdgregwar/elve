#ifndef LOADERINTERFACE_H
#define LOADERINTERFACE_H


#include <QtPlugin>
#include <QString>
#include "../Graph.h"

#define GraphLoaderPlugin_iid "ch.epfl.lap.elfe.GraphLoaderPlugin"

class GraphLoaderPlugin
{
    public:
        virtual ~GraphLoaderPlugin(){}
        virtual SharedGraph load(const QString& filepath) = 0;
        virtual QString fileFilter() = 0;
        virtual QString formatName() = 0;
};

Q_DECLARE_INTERFACE(GraphLoaderPlugin,GraphLoaderPlugin_iid)

#endif // LOADERINTERFACE_H
