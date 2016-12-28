#ifndef FILEEXPORTACTION_H
#define FILEEXPORTACTION_H

#include <QAction>
#include <interfaces/FileExporter.h>

class FileExportAction : public QAction
{
    Q_OBJECT
public:
    FileExportAction(FileExporter* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(FileExporter*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    FileExporter* mData;
};

#endif // FILEEXPORTACTION_H
