#ifndef BLIFLOADER_H
#define BLIFLOADER_H

#include <QGenericPlugin>
#include <LoaderInterface.h>

class BlifLoader : public QObject, public LoaderInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.BlifLoader")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(LoaderInterface)
public:
    BlifLoader(QObject *parent = 0);
    SharedGraph load(const QString &filepath) override;
private:
    //const Node::Description& addDescription(const NodeID& id, const Node::Type& type,const Index& ioi);
    //const Node::Description& addDescription(const NodeID& id);
    NodeDescriptions mDescriptions;

};

#endif // BLIFLOADER_H
