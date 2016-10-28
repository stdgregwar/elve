#ifndef BLIFLOADER_H
#define BLIFLOADER_H

#include <QGenericPlugin>
#include <GraphLoaderPlugin.h>

class BlifLoader : public QObject, public GraphLoaderPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.BlifLoader")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(GraphLoaderPlugin)
public:
    BlifLoader(QObject *parent = 0);
    SharedGraph load(const QString &filepath) override;
    QString fileFilter() override {
        return tr("Blif Files (*.blif)");
    }
    QString formatName() override {
        return tr("BLIF");
    }
private:
    NodeDescriptions mDescriptions;

};

#endif // BLIFLOADER_H
