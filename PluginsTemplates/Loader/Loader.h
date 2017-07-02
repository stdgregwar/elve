#pragma once

#include <QGenericPlugin>
#include <LoaderPlugin.h>

// TODO : rename the class to fit your plugin name
class LoaderName : public Elve::LoaderPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.LoaderName") // TODO : rename as well
    Q_INTERFACES(Elve::LoaderPlugin)
public:
    LoaderName();
    Elve::SharedGraph load(const QString &filepath) override;
    // TODO : replace the strings to setup GUI name, file extensions target (see QT doc), and command name
    ELVE_LOADER(LoaderName,"Loader Name in GUI","File extension filter (*.ext)","loader_cli_name")
};
