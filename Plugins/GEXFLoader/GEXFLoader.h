#ifndef BLIFLOADER_H
#define BLIFLOADER_H

#include <QGenericPlugin>
#include <GraphLoaderPlugin.h>


class GEXFLoader : public QObject, public Elve::GraphLoaderPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.GEXFLoader")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(Elve::GraphLoaderPlugin)
public:
    GEXFLoader(QObject *parent = 0){};
    Elve::SharedGraph load(const QString &filepath) override;
    ELVE_LOADER(GEXFLoader,"GEXF","GEXF Files (*.gexf)","gexf")
};

#endif // BLIFLOADER_H
