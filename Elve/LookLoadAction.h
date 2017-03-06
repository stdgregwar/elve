#pragma once

#include <QAction>


namespace Elve {

class LookFactoryPlugin;

class LookLoadAction : public QAction
{
    Q_OBJECT
public:
    LookLoadAction(LookFactoryPlugin* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(LookFactoryPlugin*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    LookFactoryPlugin* mData;
};

}
