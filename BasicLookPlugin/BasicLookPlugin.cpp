#include "BasicLookPlugin.h"
#include <GraphWidget.h>
#include <QDebug>
#include <QDomDocument>
#include <QRegularExpression>
#include <QFile>
#include "BasicNodeLook.h"
#include "BasicEdgeLook.h"

using namespace Elve;

BasicLookPlugin::BasicLookPlugin() {
    //load default colors
}

NodeLook* BasicLookPlugin::node(const Node &n) {
    return new BasicNodeLook(*this,n);
}

EdgeLook* BasicLookPlugin::edge(const NodeLook &ancestor, const NodeLook &children) {
    return new BasicEdgeLook(ancestor,children);
}

OrientationHint BasicLookPlugin::orientationHint() const {
    return BOTTOMUP;
}

void setBodyColor(QDomElement elem, const QColor& color)
{
    // if it has the tagname then overwritte desired attribute
    QString id = elem.attribute("id");
    if (id == "body")
    {
        QString style = elem.attribute("style");
        style.replace(QRegularExpression("#([0-9a-fA-F]{3}){1,2}") //Find all colors
                      ,color.name());
        elem.setAttribute("style", style);
    }
    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++)
    {
        if (!elem.childNodes().at(i).isElement())
        {
            continue;
        }
        setBodyColor(elem.childNodes().at(i).toElement(), color);
    }
}

QSvgRenderer* BasicLookPlugin::renderer(const QString& filename, const QColor& color) {

    auto it = mRenderers.find({filename,color});
    if(it != mRenderers.end()) {
        return it.value();
    } else {
        if(!color.isValid()) {
            //Use default colors
            QSvgRenderer* rend = new QSvgRenderer(filename,this);
            return mRenderers.insert({filename,color},rend).value();
        } else {
            //Replace fill and stroke of the body element
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QByteArray baData = file.readAll();
            // load svg contents to xml document and edit contents
            QDomDocument doc;
            doc.setContent(baData);
            // recurivelly change color
            setBodyColor(doc.documentElement(), color);
            // create svg renderer with edited contents
            QSvgRenderer* svgRenderer = new QSvgRenderer(doc.toByteArray(),this);
            return mRenderers.insert({filename,color},svgRenderer).value();
        }
    }
}
