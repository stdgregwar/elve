#ifndef LOADERINTERFACE_H
#define LOADERINTERFACE_H


#include <QtPlugin>
#include <QString>
#include "../Graph.h"

#define LoaderInterface_iid "ch.epfl.lap.elfe.LoaderInterface"
class LoaderInterface
{
    public:
        virtual ~LoaderInterface(){}
        virtual SharedGraph load(const QString& filepath) = 0;
};

Q_DECLARE_INTERFACE(LoaderInterface,LoaderInterface_iid)

#endif // LOADERINTERFACE_H
