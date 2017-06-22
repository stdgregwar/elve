#pragma once

#include <interfaces/LookFactoryPlugin.h>
#include <QPair>
#include <QSvgRenderer>
#include <QColor>
#include <QString>

class BasicNodeLook;
class BasicEdgeLook;

inline uint qHash(const QColor& key) {
    return key.rgba64();
}

class BasicLookPlugin : public Elve::LookFactoryPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.basicLook")
    Q_INTERFACES(Elve::LookFactoryPlugin)
public:
    ELVE_LOOK(BasicLookPlugin,"Basic","basic")
    Elve::NodeLook* node(const Elve::Node& n) override;
    Elve::EdgeLook* edge(const Elve::NodeLook& ancestor,Elve::Index outi, const Elve::NodeLook& children, Elve::Index ini) override;
    Elve::OrientationHint orientationHint() const override;
    QSvgRenderer* renderer(const QString& filename, const QColor& color);
private:
    QHash<QPair<QString,QColor>,QSvgRenderer*> mRenderers;
};
