#include "GraphWidget.h"

#include <QTimerEvent>
#include <QDebug>
#include <cmath>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QPaintEvent>
#include <QTimeLine>
#include <QGridLayout>

#include <unordered_map>
#include <random>
#include <chrono>

#include "Point.h"

#define SS 40960000

namespace Elve {

using namespace std;

constexpr int PLAYICONSIZE = 48;

std::array<QColor,10> GraphWidget::mSelectionColors = {
    Qt::blue,
    Qt::red,
    Qt::yellow,
    Qt::green,
    QColor(0, 102, 0), //Dark green
    QColor(255, 153, 0), //Orange
    QColor(255, 0, 102), //Purple
    QColor(0, 0, 255), //Dark blue
    QColor(255, 102, 255), //Pink
    QColor(128, 0, 0) //Bordeaux
};

GraphWidget::GraphWidget(QWidget* parent, GraphWidgetListener* listener) : QGraphicsView(parent),
    mScene(new QGraphicsScene(-SS,-SS,SS*2,SS*2,this)),
    mDrag(false),
    mScale(1),
    mTargetScale(1),
    mBehaviour(new Behaviour(this)),
    //mEdgesPath(new QGraphicsPathItem()),
    mListener(listener),
    mTimerId(-1),
    mSim(true)
{
    mPlayPauseIcon = new QGraphicsPixmapItem();
    mPlayPauseIcon->setPixmap(QIcon(":resources/play.svg").pixmap(PLAYICONSIZE,PLAYICONSIZE));
    mPlayPauseIcon->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    mPlayPauseIcon->setPos(20,50);
    mPlayPauseIcon->setZValue(2);
    mScene->addItem(mPlayPauseIcon);

    mSelectionBox = new QComboBox(this);
    mSelectionBox->setGeometry(20,20,100,20);
    for(int i = 0; i < mSelectionColors.size(); i++) {
        mSelectionBox->addItem("Mask " + QString::number(i));
    }

    connect(mSelectionBox,SIGNAL(activated(int)),this,SLOT(setCurrentMask(int)));
    connect(this,SIGNAL(maskChanged(int)),mSelectionBox,SLOT(setCurrentIndex(int)));

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setGeometry(0,0,1280,720);
    setScene(mScene);
    setBackgroundBrush(QBrush(QColor(59,58,58), Qt::SolidPattern));

}

void GraphWidget::start() {
    mTimerId = startTimer(1000/60);
    show();
}

void GraphWidget::stop() {
    if(mTimerId != -1) {
        killTimer(mTimerId);
        mTimerId = -1;
    }
    hide();
}

void GraphWidget::toggleSim(bool sim) {
    mSim = !sim;
    mPlayPauseIcon->setPixmap(
                QIcon(mSim ? ":resources/play.svg" : ":resources/pause.svg")
                .pixmap(PLAYICONSIZE,PLAYICONSIZE));
}

void GraphWidget::setCurrentMask(int i) {
    mGraph->setMask(i);
    updateSelectionColor();
}

void GraphWidget::fit() {
    //fitInView(mScene->itemsBoundingRect(),Qt::KeepAspectRatio);
    fitInView(mGraph->layout()->system().sizeHint(),Qt::KeepAspectRatio);
}

void GraphWidget::init()
{
}

void GraphWidget::showEvent(QShowEvent *event)
{
    QGraphicsView::showEvent(event);
    init();
}

void GraphWidget::clear() {
    mEdges.clear();
    mNodes.clear();
}

void GraphWidget::setGraph(SharedEGraph graph, unsigned quickTicks) {
    unsetGraph();

    mListener->graphChanged(mGraph,graph);
    mGraph = graph;
    mGraph->setView(this);
    if(graph->layout() && graph->look()) {
        //graph->applyLayout();
        reflect(graph->layout()->system(),graph->graph(),graph->look());
        quickSim(quickTicks);

        mScene->setSceneRect(graph->layout()->system().sizeHint());
    }
    updateSelectionColor();
}

QGraphicsScene* GraphWidget::scene() {
    return mScene;
}

void GraphWidget::quickSim(unsigned ticks)
{
    if(mGraph && mGraph->layout())
        mGraph->layout()->quickSim(ticks);
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect){
    QGraphicsView::drawBackground(painter,rect);
    if(mGraph->layout()) {
        painter->setPen(QPen(Qt::gray));
        //mGraph->layout()->system().debug(painter);
        painter->drawRect(mGraph->layout()->system().sizeHint());
    }
    //painter->fillRect(rect,Qt::CrossPattern);
}

void GraphWidget::drawForeground(QPainter *painter, const QRectF &rect) {
    //painter->drawText(QPointF{20,20},QString("selection : %1").arg(QString::number(mGraph->mask())));
    QImage img = QIcon(":resources/lock.svg").pixmap(QSize(64,64)).toImage();
    for(const PointsByID::value_type& p : mGraph->layout()->system().pinnedPoints()) {
        const QVector2D& pos = p.second->pos();
        painter->drawImage(QPointF(pos.x(),pos.y()),img);
    }
}

void GraphWidget::paintEvent(QPaintEvent* ev) {
    mPlayPauseIcon->setPos(mapToScene(20,50));
    QGraphicsView::paintEvent(ev);
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    const QPoint degrees = event->angleDelta()/ 8;
    const int steps = degrees.y() / 15;
    const float scalebase = sqrt(2);
    float s = pow(scalebase,steps);
    mTargetScale*=s;
    event->accept();
    //QGraphicsView::wheelEvent(event);
}

void GraphWidget::borderSelect() {
    setBehaviour(new BorderSelect(this));
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if(!mBehaviour || !mBehaviour->mousePressEvent(event)) {
        if(event->button() == Qt::RightButton) {
            QList<QGraphicsItem*> items = mScene->items(mapToScene(event->pos()));
            NodeIDSet names;
            for(QGraphicsItem* i : items) {
                NodeLook* n = dynamic_cast<NodeLook*>(i);
                if(n) {
                    names.insert(n->node().id());
                }
            }
            SelectionMode mode = CLEAR;
            if(event->modifiers() & Qt::ShiftModifier) mode = ADD;
            if(event->modifiers() & Qt::ControlModifier) mode = SUB;
            select(names,mode);
        }
        QGraphicsView::mousePressEvent(event);
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(!mBehaviour || !mBehaviour->mouseReleaseEvent(event)) {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!mBehaviour || !mBehaviour->mouseMoveEvent(event)) {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent* event) { //TEMP ungroup feature
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        mGraph->setMask(event->key()-Qt::Key_0);
        emit maskChanged(event->key()-Qt::Key_0);
        updateSelectionColor();
        mListener->selectionChanged(mGraph);
    }
}

void GraphWidget::quickSim() {
    quickSim(500);
}

void GraphWidget::group() {
    qDebug() << "grouping!";
    //group(mGraph->currentSelection());
    mListener->runCommand(QString("group -m %1").arg(mGraph->mask()));
}

void GraphWidget::ungroup() {
    //ungroup(mGraph->currentSelection());
    mListener->runCommand(QString("ungroup -m %1").arg(mGraph->mask()));
}

void GraphWidget::toggleSelection() {
    Selection& s = mGraph->currentSelection();
    QString cmd = QString("select %1").arg(QString::number(mGraph->mask()));
    if(s.size() == 0) {
        cmd += " -a";
    } else {
        //s.clear();
        cmd += " -c";

    }
    mListener->runCommand(cmd);
    //updateSelectionColor();
}

void GraphWidget::ungroup(const NodeIDSet& names) {
    setGraph(mGraph->ungroup(names),0);
}

void GraphWidget::group(const Selection &names, const NodeName &groupName) {
    if(names.empty()) return;
    NodeIDSet inputs;
    NodeIDSet nonio;
    NodeIDSet outputs;
    for(const NodeID& id : names) {
        qDebug() << "id" << id;
        const Node& nd = mGraph->graph()->nodes().at(id);
        switch(nd.type()) {
        case INPUT:
            inputs.insert(id);
            break;
        case OUTPUT:
            outputs.insert(id);
            break;
        default:
            nonio.insert(id);
            break;
        }
    }
    SharedEGraph eg = mGraph->group(nonio,groupName)
            ->group(inputs,groupName)
            ->group(outputs,groupName);
    setGraph(eg,0);
}

void GraphWidget::tick(float dt, bool update)
{

    if(mGraph->layout()) {
        mGraph->layout()->tick(dt,update);
        /*QPainterPath p;
        std::for_each(mEdges.begin(),mEdges.end(),[&p](EdgeLook*& e){
            e->addToPath(p);
        });
        mEdgesPath->setPath(p);*/
    }
}

void GraphWidget::timerEvent(QTimerEvent *e)
{
    if(mSim) tick(0.25);


    updateEdges();

    qreal oldScale= mScale;
    mScale = 0.8*mScale+0.2*mTargetScale;
    qreal factor = mScale/oldScale;
    if(abs(1-factor)>1e-3) {
        scale(factor,factor);
    }
}

void GraphWidget::setBehaviour(Behaviour* b) {
    if(mBehaviour) {
        mBehaviour->onEnd();
        delete mBehaviour;
    }
    b->onStart();
    mBehaviour = b;
}

void GraphWidget::setLayout(const SharedLayout& l) {
    mGraph->setLayout(l);
    setSceneRect(l->system().sizeHint());
    //reflect(mGraph->layout()->system(),mGraph->graph());
}

void GraphWidget::clearScene() {
    clearEdgesPaths();
    mScene->removeItem(mPlayPauseIcon);
    mScene->clear();
    mScene->addItem(mPlayPauseIcon);
    for_each(mEdges.begin(),mEdges.end(),[](EdgeLook* e){delete e;});
    mEdges.clear();
    mNodes.clear();
}

void GraphWidget::reflect(System &sys, SharedGraph g, SharedLook lf) {
    clearScene();

    qDebug() << "Reflecting" << lf->lookName();

    setSceneRect(sys.sizeHint());

    unordered_map<NodeID,NodeLook*> looks;

    for(auto& nbi : g->nodes()) {
        const Node& n = nbi.second;
        NodeLook* ni = lf->node(n);
        looks[n.id()] = ni;
        mNodes.push_back(ni);
        Point* p = sys.point(n.id());
        p->clearMovables();
        p->addMovable(ni);
        mScene->addItem(ni);
    }

    for(auto& nbi : g->nodes()) {
        const Node& n = nbi.second;
        for(const Node::Connexion& c : n.fanIn()) {
            const NodeLook& al = *looks.at(c.node->id());
            const NodeLook& ll = *looks.at(n.id());
            EdgeLook* ei = lf->edge(al,c.from,ll,c.to);
            //Point* ep = sys.point(c.node->id());
            mEdges.push_back(ei);
        }
    }
    updateSelectionColor();
    qDebug() << "Reflected";
}

/*
 * BORDER SELECT BEHAVIOUR
 */

GraphWidget::BorderSelect::BorderSelect(GraphWidget* parent) : Behaviour(parent) {

}

void GraphWidget::BorderSelect::onStart() {
    QPen dashPen = QPen(QColor(200,200,200));
    dashPen.setStyle(Qt::DashLine);
    mRectangle = gw.mScene->addRect({0,0,0,0},
                                    dashPen);
    mCross = gw.mScene->addPath(QPainterPath(),dashPen);
}

bool GraphWidget::BorderSelect::mousePressEvent(QMouseEvent *event) {
    gw.mScene->removeItem(mCross);
    //delete mCross;
    mCross = nullptr;
    mRectangle->setPos(gw.mapToScene(event->pos()));
    return true;
}


bool GraphWidget::BorderSelect::mouseReleaseEvent(QMouseEvent *event) {
    QList<QGraphicsItem*> items = gw.mScene->items(mRectangle->sceneBoundingRect());
    NodeIDSet names;
    for(QGraphicsItem* i : items) {
        NodeLook* n = dynamic_cast<NodeLook*>(i);
        if(n) {
            names.insert(n->node().id());
        }
    }
    SelectionMode mode = CLEAR;
    if(event->modifiers() & Qt::ShiftModifier) mode = ADD;
    if(event->modifiers() & Qt::ControlModifier) mode = SUB;
    gw.select(names,mode);
    //gw.group(names);
    gw.setBehaviour(new Behaviour(&gw));
    gw.mScene->removeItem(mRectangle);
    return true;
}

void GraphWidget::select(const NodeIDSet& names, SelectionMode mode) {
    if(names.empty()) return;
    QString cmd = QString("select");
    switch(mode) {
    case CLEAR:
        cmd += " -c";
        break;
    case ADD:
        cmd += " --add";
        break;
    case SUB:
        cmd += " --sub";
        break;
    default:
        break;
    }

    cmd += QString(" %1").arg(QString::number(mGraph->mask()));
    for(const NodeID& id : names) {
        cmd += " " + QString::number(id);
    }
    mListener->runCommand(cmd);
}

void GraphWidget::updateSelectionColor() {
    Selection& s = mGraph->currentSelection();
    for(NodeLook* i : mNodes) {
        if(s.count(i->node().id())) {
            i->color(mSelectionColors[mGraph->mask()]);
        } else {
            i->color(QColor());
        }
    }
}

void GraphWidget::flushPen(QPen& pen, QPainterPath& path, const QPen &newPen) {
    //Flush in pathitem
    if(path.length() > 0.f) {
        QGraphicsPathItem* pitem = new QGraphicsPathItem(path);
        mScene->addItem(pitem);
        pitem->setPen(pen);
        mEdgesPaths.push_back(pitem);
        path = QPainterPath(); //clear path
    }
    pen = newPen;
}

void GraphWidget::clearEdgesPaths() {
    for(QGraphicsPathItem* p : mEdgesPaths) {
        mScene->removeItem(p);
        delete p;
    }
    mEdgesPaths.clear();
}

void GraphWidget::updateEdges() {
    clearEdgesPaths();

    QPen pen;
    QPainterPath path;
    for(EdgeLook* e : mEdges) {
        if(pen != e->pen()) {
            flushPen(pen,path,e->pen());
        }
        e->addToPath(path);
    }
    flushPen(pen,path,QPen());
}

bool GraphWidget::BorderSelect::mouseMoveEvent(QMouseEvent *event) {
    float inf = 800000; //TODO cross
    QPointF pos = gw.mapToScene(event->pos());
    if(mCross) {

    } else {
        QPointF origin = mRectangle->pos();
        mRectangle->setRect(0,0,
                            (pos.x()-origin.x()),(pos.y()-origin.y()));
    }
    return true;
}

const SharedEGraph& GraphWidget::graph() const
{
    return mGraph;
}

void GraphWidget::BorderSelect::onEnd() {
    if(mCross) {
        gw.mScene->removeItem(mCross);
        //delete mCross;
    }
    //gw.mScene->removeItem(mRectangle);
    //delete mRectangle;
}

void GraphWidget::unsetGraph() {
    if(mGraph) {
        mGraph->setView(nullptr);
        if(mGraph->layout()) {
            mGraph->layout()->system().clearMovables();
        }
    }
}

GraphWidget::~GraphWidget() {
    unsetGraph();
}

}
