#pragma once

#include <QGenericPlugin>
#include <GraphLoaderPlugin.h>


class LoaderName : public QObject, public Elve::GraphLoaderPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.LoaderName")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(Elve::GraphLoaderPlugin)
public:
    LoaderName(QObject *parent = 0);
    Elve::SharedGraph load(const QString &filepath) override;
    ELVE_LOADER(LoaderName,"Loader Name in GUI","File extension filter (*.ext)","loader_cli_name")
};
