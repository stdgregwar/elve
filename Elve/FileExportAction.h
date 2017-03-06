#ifndef FILEEXPORTACTION_H
#define FILEEXPORTACTION_H

#include <QAction>
#include <interfaces/FileExporterPlugin.h>


namespace Elve {

class FileExportAction : public QAction
{
    Q_OBJECT
public:
    FileExportAction(FileExporterPlugin* data, const QString& text,QObject* parent = nullptr) : QAction(text,parent),mData(data)
    {
        connect(this,SIGNAL(triggered()),this,SLOT(onTriggered()));
    }
signals:
    void triggered(FileExporterPlugin*);
public slots:
    void onTriggered() {
        emit triggered(mData);
    }
private:
    FileExporterPlugin* mData;
};

}
#endif // FILEEXPORTACTION_H
