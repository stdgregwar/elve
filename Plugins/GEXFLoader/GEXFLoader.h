#ifndef BLIFLOADER_H
#define BLIFLOADER_H

#include <QGenericPlugin>
#include <LoaderPlugin.h>


class GEXFLoader : public QObject, public Elve::LoaderPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.GEXFLoader")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(Elve::LoaderPlugin)
public:
    GEXFLoader(QObject *parent = 0){}
    Elve::SharedGraph load(const QString &filepath) override;
    ELVE_LOADER(GEXFLoader,"GEXF","GEXF Files (*.gexf)","gexf")
};

#endif // BLIFLOADER_H
