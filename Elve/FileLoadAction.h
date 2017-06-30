#ifndef FILELOADACTION_H
#define FILELOADACTION_H

#include <QAction>
#include <interfaces/LoaderPlugin.h>

namespace Elve {

class FileLoadAction : public QAction
{
    Q_OBJECT
public:
    FileLoadAction(LoaderPlugin* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(LoaderPlugin*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    LoaderPlugin* mData;
};

}

#endif // FILELOADACTION_H
