#ifndef FILELOADACTION_H
#define FILELOADACTION_H

#include <QAction>
#include <interfaces/GraphLoaderPlugin.h>

class FileLoadAction : public QAction
{
    Q_OBJECT
public:
    FileLoadAction(GraphLoaderPlugin* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(GraphLoaderPlugin*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    GraphLoaderPlugin* mData;
};

#endif // FILELOADACTION_H
