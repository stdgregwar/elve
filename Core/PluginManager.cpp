#include "PluginManager.h"
#include <QDir>
#include <QMessageBox>
#include <QDebug>

PluginManager::PluginManager()
{
    //loadPlugins(".");
}

const Loaders &PluginManager::loaders() const
{
    return mLoaders;
}

const Layouts& PluginManager::layouts() const {
    return mLayouts;
}

template <class T>
void _load(const QString& path, const QString& type, QList<T*>& toFill) {
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
    for(LayoutPlugin* l : mLayouts) {
        if(l->layoutName() == name) {
            return l;
        }
    }
    return nullptr; //TODO
}

void PluginManager::load(const QString& path)
{
    //For loaders
    _load(path+"/loaders","Graph Loader",mLoaders);
    //For layouts
    _load(path+"/layouts","Layout",mLayouts);
}