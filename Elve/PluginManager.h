#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <interfaces/GraphLoaderPlugin.h>
#include <interfaces/LayoutPlugin.h>
#include <QPluginLoader>
#include <QMap>


typedef QList<GraphLoaderPlugin*> Loaders;
typedef QList<LayoutPlugin*> Layouts;

/**
 * @brief Class responsible for collecting and storing all the plugins present in the given folder
 */
class PluginManager
{
public:
    PluginManager(const QString& path);
    const Loaders& loaders() const;
    const Layouts& layouts() const;
    LayoutPlugin* getLayout(const QString& name) const;
private:
    void loadPlugins(const QString& path);

    Loaders mLoaders;
    Layouts mLayouts;
};

#endif // PLUGINMANAGER_H
