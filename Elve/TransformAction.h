#pragma once

#include <QAction>
#include <interfaces/GraphTransformPlugin.h>

namespace Elve {

class TransformAction : public QAction
{
    Q_OBJECT
public:
    TransformAction(TransformPlugin* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(TransformPlugin*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    TransformPlugin* mData;
};

}
