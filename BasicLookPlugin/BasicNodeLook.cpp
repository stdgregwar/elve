#include "BasicNodeLook.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsRectItem>

using namespace Elve;

static QColor baseColor(34,34,34);

static std::unordered_map<NodeType,QString> pixmaps{
    {NodeType::CLUSTER,":/resources/cluster.svg"},
    {NodeType::NODE,":/resources/node.svg"},
    {NodeType::INPUT,":/resources/input.svg"},
    {NodeType::OUTPUT,":/resources/output.svg"},
    {NodeType::INPUT_CLUSTER,":/resources/input.svg"},
    {NodeType::OUTPUT_CLUSTER,":/resources/output.svg"}};

BasicNodeLook::BasicNodeLook(BasicLookPlugin& look,const Node& node) : mLook(look),NodeLook(node)
{


    if(node.type() == CLUSTER) {
        setToolTip(QString("Name : %1\n"
                           "ID   : %2\n"
                           "nodeCount : %3").arg(node.name().c_str()).arg(node.id()).arg(node.data().dependencies().size()));
    } else {
        QJsonObject truthtable = node.properties()["truthtable"].toObject();
        QString table;
        for(QJsonObject::const_iterator it = truthtable.constBegin();
                it != truthtable.constEnd();
                it++) {
            table += it.key() + " ";
            table += it.value().toString() + "\n";
        }
        setToolTip(QString("Name : %1\n"
                           "ID   : %2\n"
                           "truthtable :\n %3\n"
                           "ancestorsCount : %4").arg(node.name().c_str()).arg(node.id()).arg(table).arg(node.data().dependencies().size()));
    } //TODO move all of this to a information module or smth



    setZValue(1);

    QSvgRenderer* r = look.renderer(pixmaps[node.type()],QColor());
    mItem = new QGraphicsSvgItem(this);
    mItem->setCachingEnabled(false);
    mItem->setSharedRenderer(r);
    mItem->setPos(-r->defaultSize().width()/2,-r->defaultSize().height()/2);
    addToGroup(mItem);
    //setBrush(QBrush(QColor(240,240,240)));
    setAcceptedMouseButtons(Qt::MouseButton::AllButtons);
}

void BasicNodeLook::onColorChange(const QColor& col) {
    mItem->setSharedRenderer(mLook.renderer(pixmaps[node().type()],col));
}

void BasicNodeLook::onColorReset() {
    mItem->setSharedRenderer(mLook.renderer(pixmaps[node().type()],QColor()));
}
