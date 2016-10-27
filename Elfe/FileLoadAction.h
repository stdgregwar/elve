#ifndef FILELOADACTION_H
#define FILELOADACTION_H

#include <QAction>
#include <interfaces/GraphLoader.h>

class FileLoadAction : public QAction
{
    Q_OBJECT
public:
    FileLoadAction(GraphLoader* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(GraphLoader*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    GraphLoader* mData;
};

#endif // FILELOADACTION_H
