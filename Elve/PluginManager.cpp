#include "PluginManager.h"
#include <QDir>
#include <QMessageBox>
#include <QDebug>

PluginManager::PluginManager(const QString &path)
{
    loadPlugins(path);
}

const Loaders &PluginManager::loaders() const
{
    return mLoaders;
}

const Layouts& PluginManager::layouts() const {
    return mLayouts;
}

template <class T>
void load(const QString& path, const QString& type, QList<T*>& toFill) {
    QDir dir(path);
    for(const QFileInfo& info : dir.entryInfoList(QDir::Files)) {
        qDebug() << "Trying to load" << info.baseName();
        QPluginLoader* qpl = new QPluginLoader(info.filePath());
        if(!qpl->load()) {
            QMessageBox("Warning","Could not load " + info.baseName() + " : \n" + qpl->errorString(),QMessageBox::Warning,0,0,0).exec();
            qDebug() << qpl->errorString() << endl;
        } else {
            QObject* obj = qpl->instance();
            T* interface = qobject_cast<T*>(obj);
            if(interface) {
                toFill.push_back(interface);
            } else {
                QMessageBox("Warning","Could not load " + info.baseName() + " : \n Plugin is not a " + type,QMessageBox::Warning,0,0,0).exec();
            }
        }
        delete qpl;
    }
}

LayoutPlugin* PluginManager::getLayout(const QString& name) const
{
    return nullptr; //TODO
}

void PluginManager::loadPlugins(const QString& path)
{
    //For loaders
    load(path+"/loaders","Graph Loader",mLoaders);
    //For layouts
    load(path+"/layouts","Layout",mLayouts);
}
