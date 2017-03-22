#pragma once

#include <QGenericPlugin>
#include <GraphLoaderPlugin.h>

class DivisorLoader : public QObject, public Elve::GraphLoaderPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.DivisorLoader")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(Elve::GraphLoaderPlugin)
public:
    DivisorLoader(QObject *parent = 0);
    Elve::SharedGraph readBlif(const QString& filepath);
    Elve::SharedGraph load(const QString &filepath) override;
    ELVE_LOADER(DivisorLoader,"Divisors","JSON Files (*.json)","divisors")
};