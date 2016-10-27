#ifndef LOADERINTERFACE_H
#define LOADERINTERFACE_H


#include <QtPlugin>
#include <QString>
#include "../Graph.h"

#define GraphLoader_iid "ch.epfl.lap.elfe.LoaderInterface"

class GraphLoader
{
    public:
        virtual ~GraphLoader(){}
        virtual SharedGraph load(const QString& filepath) = 0;
        virtual QString fileFilter() = 0;
        virtual QString formatName() = 0;
};

Q_DECLARE_INTERFACE(GraphLoader,GraphLoader_iid)

#endif // LOADERINTERFACE_H
