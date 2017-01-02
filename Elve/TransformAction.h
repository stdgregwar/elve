#pragma once

#include <QAction>
#include <interfaces/GraphTransformPlugin.h>

class TransformAction : public QAction
{
    Q_OBJECT
public:
    TransformAction(GraphTransformPlugin* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(GraphTransformPlugin*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    GraphTransformPlugin* mData;
};
