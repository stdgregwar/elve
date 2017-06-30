#include "BasicNodeLook.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsRectItem>

#include <NodeDescriptor.h>

using namespace Elve;

static QColor baseColor(34,34,34);

static std::unordered_map<int,QString> pixmaps{
    {NodeType::CLUSTER,":/resources/cluster.svg"},
    {NodeType::NODE,":/resources/node.svg"},
    {NodeType::INPUT,":/resources/input.svg"},
    {NodeType::OUTPUT,":/resources/output.svg"},
    {NodeType::INPUT_CLUSTER,":/resources/input.svg"},
    {NodeType::OUTPUT_CLUSTER,":/resources/output.svg"}};

BasicNodeLook::BasicNodeLook(BasicLookPlugin& look,const Node& node) : mLook(look),NodeLook(node)
{
    setToolTip(NodeDescriptor::nodeDescription(node));


    if(node.properties().contains("color")) {
        mBaseColor = QColor(node.properties().value("color").toString());
    } else {
        mBaseColor = QColor();
    }

    setZValue(1);

    QSvgRenderer* r = look.renderer(pixmaps[node.type()],QColor());
    mItem = new QGraphicsSvgItem(this);
    mItem->setCachingEnabled(true);
    mItem->setSharedRenderer(r);
    mItem->setPos(-r->defaultSize().width()/2,-r->defaultSize().height()/2);
    addToGroup(mItem);
    //setBrush(QBrush(QColor(240,240,240)));
    setAcceptedMouseButtons(Qt::MouseButton::AllButtons);
}

void BasicNodeLook::onColorChange(const QColor& col) {
    QColor tcol = col;
    if(mBaseColor.isValid()) {
        tcol = QColor((col.red()+mBaseColor.red())/2,
                      (col.green()+mBaseColor.green())/2,
                      (col.blue()+mBaseColor.blue())/2);
    }
    mItem->setSharedRenderer(mLook.renderer(pixmaps[node().type()],tcol));
}

void BasicNodeLook::onColorReset() {
    mItem->setSharedRenderer(mLook.renderer(pixmaps[node().type()],mBaseColor));
}
