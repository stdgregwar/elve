#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <interfaces/GraphLoaderPlugin.h>
#include <interfaces/LayoutPlugin.h>
#include <interfaces/FileExporterPlugin.h>
#include <interfaces/GraphTransformPlugin.h>
#include <QPluginLoader>
#include <QMap>
#include <QList>

#include "Singleton.h"

typedef QList<GraphLoaderPlugin*> Loaders;
typedef QList<GraphTransformPlugin*> Transforms;
typedef QList<FileExporterPlugin*> Exporters;
typedef QList<LayoutPlugin*> Layouts;

/**
 * @brief Class responsible for collecting and storing all the plugins present in the given folder
 */
class PluginManager : public Singleton<PluginManager>
{
    friend Singleton<PluginManager>;
public:
    const Loaders& loaders() const;
    const Exporters& exporters() const;
    const Layouts& layouts() const;
    const Transforms& transforms() const;
    SharedLayout getLayout(const QString& name) const;
    SharedLayout defaultLayout() const;
    void load(const QString& path);
private:
    PluginManager();
    Loaders mLoaders;
    Layouts mLayouts;
    Exporters mExporters;
    Transforms mTransforms;
};

#endif // PLUGINMANAGER_H
