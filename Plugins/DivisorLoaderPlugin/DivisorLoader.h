#pragma once

#include <QGenericPlugin>
#include <LoaderPlugin.h>

class DivisorLoader : public Elve::LoaderPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.DivisorLoader")
    Q_INTERFACES(Elve::LoaderPlugin)
public:
    DivisorLoader();
    Elve::SharedGraph readBlif(const QString& filepath);
    Elve::SharedGraph load(const QString &filepath) override;
    ELVE_LOADER(DivisorLoader,"Divisors","JSON Files (*.json)","divisors")
};
