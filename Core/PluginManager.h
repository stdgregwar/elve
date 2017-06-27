#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <interfaces/GraphLoaderPlugin.h>
#include <interfaces/LayoutPlugin.h>
#include <interfaces/FileExporterPlugin.h>
#include <interfaces/GraphTransformPlugin.h>
#include <interfaces/LookFactoryPlugin.h>
#include <QPluginLoader>
#include <QMap>
#include <QList>

#include "Singleton.h"

namespace Elve {

typedef QList<LoaderPlugin*> Loaders;
typedef QList<TransformPlugin*> Transforms;
typedef QList<FileExporterPlugin*> Exporters;
typedef QList<LayoutPlugin*> Layouts;
typedef QList<LookFactoryPlugin*> Looks;

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
    const Looks& looks() const;
    SharedLayout getLayout(const QString& name) const;
    SharedLayout defaultLayout() const;
    SharedLook defaultLook() const;
    void load(const QString& path);
private:
    PluginManager();
    Loaders mLoaders;
    Layouts mLayouts;
    Exporters mExporters;
    Transforms mTransforms;
    Looks mLooks;
};

}
#endif // PLUGINMANAGER_H
