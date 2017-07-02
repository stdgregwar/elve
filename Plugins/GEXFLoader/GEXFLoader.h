#ifndef BLIFLOADER_H
#define BLIFLOADER_H

#include <QGenericPlugin>
#include <LoaderPlugin.h>


class GEXFLoader : public Elve::LoaderPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.GEXFLoader")
    Q_INTERFACES(Elve::LoaderPlugin)
public:
    GEXFLoader(QObject *parent = 0){}
    Elve::SharedGraph load(const QString &filepath) override;
    ELVE_LOADER(GEXFLoader,"GEXF","GEXF Files (*.gexf)","gexf")
};

#endif // BLIFLOADER_H
