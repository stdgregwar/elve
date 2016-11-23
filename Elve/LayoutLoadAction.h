#ifndef LAYOUTLOADACTION_H
#define LAYOUTLOADACTION_H

#include <QAction>

class LayoutPlugin;

class LayoutLoadAction : public QAction
{
    Q_OBJECT
public:
    LayoutLoadAction(LayoutPlugin* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(LayoutPlugin*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    LayoutPlugin* mData;
};

#endif // LAYOUTLOADACTION_H
