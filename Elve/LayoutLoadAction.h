#ifndef LAYOUTLOADACTION_H
#define LAYOUTLOADACTION_H

#include <QAction>

class LayoutPluginFactory;

class LayoutLoadAction : public QAction
{
    Q_OBJECT
public:
    LayoutLoadAction(LayoutPluginFactory* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(LayoutPluginFactory*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    LayoutPluginFactory* mData;
};

#endif // LAYOUTLOADACTION_H
