#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <interfaces/GraphLoaderPlugin.h>
#include <interfaces/LayoutPlugin.h>
#include <QPluginLoader>
#include <QMap>
#include <QList>

#include "Singleton.h"

typedef QList<GraphLoaderPlugin*> Loaders;
typedef QList<LayoutPlugin*> Layouts;

/**
 * @brief Class responsible for collecting and storing all the plugins present in the given folder
 */
class PluginManager : public Singleton<PluginManager>
{
    friend Singleton<PluginManager>;
public:
    const Loaders& loaders() const;
    const Layouts& layouts() const;
    LayoutPlugin* getLayout(const QString& name) const;
    void load(const QString& path);
private:
    PluginManager();
    Loaders mLoaders;
    Layouts mLayouts;
};

#endif // PLUGINMANAGER_H
