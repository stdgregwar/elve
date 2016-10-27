#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <interfaces/GraphLoader.h>
#include <QPluginLoader>
#include <QMap>

typedef QList<GraphLoader*> Loaders;

/**
 * @brief Class responsible for collecting and storing all the plugins present in the given folder
 */
class PluginManager
{
public:
    PluginManager(const QString& path);
    const Loaders& loaders() const;
private:
    void loadPlugins(const QString& path);
    Loaders mLoaders;
};

#endif // PLUGINMANAGER_H
