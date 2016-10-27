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

void PluginManager::loadPlugins(const QString& path)
{
    //For loaders
    QDir dir(path+"/loaders");
    for(const QFileInfo& info : dir.entryInfoList(QDir::Files)) {
        qDebug() << "Trying to load" << info.baseName();
        QPluginLoader* qpl = new QPluginLoader(info.filePath());
        if(!qpl->load()) {
            QMessageBox("Warning","Could not load " + info.baseName() + " : \n" + qpl->errorString(),QMessageBox::Warning,0,0,0).exec();
            qDebug() << qpl->errorString() << endl;

        } else {
            QObject* obj = qpl->instance();
            GraphLoader* interface = qobject_cast<GraphLoader*>(obj);
            if(interface) {
                mLoaders.push_back(interface);
            } else {
                QMessageBox("Warning","Could not load " + info.baseName() + " : \n Plugin is not a file loader",QMessageBox::Warning,0,0,0).exec();
            }
        }
        delete qpl;
    }
}
