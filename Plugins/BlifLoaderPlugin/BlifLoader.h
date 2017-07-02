#ifndef BLIFLOADER_H
#define BLIFLOADER_H

#include <QGenericPlugin>
#include <LoaderPlugin.h>


class BlifLoader : public Elve::LoaderPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.BlifLoader")
    Q_INTERFACES(Elve::LoaderPlugin)
public:
    BlifLoader();
    Elve::SharedGraph load(const QString &filepath) override;
    ELVE_LOADER(BlifLoader,"BLIF","BLIF Files (*.blif)","blif")
};

#endif // BLIFLOADER_H
