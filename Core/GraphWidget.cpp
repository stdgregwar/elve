#include "GraphWidget.h"

#include <QTimerEvent>
#include <QDebug>
#include <cmath>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QJsonObject>
#include <QJsonArray>

#include <unordered_map>
#include <random>
#include <chrono>

#define SS 320000

using namespace std;

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

GraphWidget::GraphWidget(QWidget* parent, QString filename) : QGraphicsView(parent),
    mScene(new QGraphicsScene(-SS,-SS,SS*2,SS*2,this)),
    mDrag(false),
    mScale(1),
    mBehaviour(new Behaviour(this)),
    mEdgesPath(new QGraphicsPathItem()),
    mFilename(filename),
    mCurrentMask(0)
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setGeometry(0,0,1280,720);
    setScene(mScene);
    setBackgroundBrush(QBrush(QColor(59,58,58), Qt::SolidPattern));
    startTimer(1000/60);

    QPen p(QColor(100,100,100), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    p.setCosmetic(true);
    mEdgesPath->setPen(p);
}

void GraphWidget::fit() {
    fitInView(mScene->itemsBoundingRect(),Qt::KeepAspectRatio);
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
}

void GraphWidget::setGraph(SharedEGraph graph, unsigned quickTicks) {
    mGraph = graph;
    mGraph->setView(this);
    if(graph->layout()) {
        //graph->applyLayout();
        reflect(graph->layout()->system(),graph->graph());
        quickSim(quickTicks);
    }
}

QGraphicsScene* GraphWidget::scene() {
    return mScene;
}

void GraphWidget::quickSim(unsigned ticks)
{
    mGraph->layout()->quickSim(ticks);
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect){
    QGraphicsView::drawBackground(painter,rect);
    if(mGraph->layout()) {
        //mLayout->system().debug(painter);
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    const QPoint degrees = event->angleDelta()/ 8;
    const int steps = degrees.y() / 15;
    const float scalebase = sqrt(2);
    float s = pow(scalebase,steps);
    scale(s,s);
    mScale*=s;
    event->accept();
    QGraphicsView::wheelEvent(event);
}

void GraphWidget::borderSelect() {
    setBehaviour(new BorderSelect(this));
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if(!mBehaviour || !mBehaviour->mousePressEvent(event)) {
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
    QList<QGraphicsItem*> items = mScene->items(mapToScene(event->pos()));
    NodeIDs names;
    for(QGraphicsItem* i : items) {
        NodeItem* n = dynamic_cast<NodeItem*>(i);
        if(n) {
            names.push_back(n->id());
        }
    }
    ungroup(names);
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    if(event->key() == Qt::Key_Q) {
        if(mGraph->layout()) {
            mGraph->layout()->quickSim(400);
        }
    } else if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        mCurrentMask = event->key()-Qt::Key_0;
        updateSelectionColor();
    }
}

void GraphWidget::toggleSelection() {
    Selection& s = mGraph->selection(mCurrentMask);
    if(s.size() == 0) {
        for(const auto& p : mGraph->graph()->nodes()) {
            s.add(p.first);
        }
    } else {
        s.clear();
    }
    updateSelectionColor();
}

void GraphWidget::ungroup(const NodeIDs& names) {
    setGraph(mGraph->ungroup(names),0);
}

void GraphWidget::group(const NodeIDSet &names, const NodeName &groupName) {
    NodeIDSet inputs;
    NodeIDSet nonio;
    NodeIDSet outputs;
    for(const NodeID& id : names) {
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
        QPainterPath p;
        std::for_each(mEdges.begin(),mEdges.end(),[&p](EdgeItem*& e){
            e->doPath(p);
        });
        mEdgesPath->setPath(p);
    }
}

void GraphWidget::timerEvent(QTimerEvent *e)
{
    tick(0.25);
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
    //reflect(mGraph->layout()->system(),mGraph->graph());
}

void GraphWidget::clearScene() {
    mScene->removeItem(mEdgesPath);
    mScene->clear();
    mScene->addItem(mEdgesPath);
    for_each(mEdges.begin(),mEdges.end(),[](EdgeItem* e){delete e;});
    mEdges.clear();
    mNodes.clear();
}

void GraphWidget::reflect(System &sys,SharedGraph g) {
    clearScene();
    for(auto& nbi : g->nodes()) {
        const Node& n = nbi.second;
        NodeItem* ni = new NodeItem(n.data());
        mNodes.push_back(ni);
        Point* p = sys.point(n.id());
        p->addMovable(ni);
        mScene->addItem(ni);
        for(const Node* an : n.ancestors()) {
            EdgeItem* ei = new EdgeItem(1);
            Point* ep = sys.point(an->id());
            p->addMovable(ei->getHandle(0));
            ep->addMovable(ei->getHandle(1));
            mEdges.push_back(ei);
        }
    }
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
        NodeItem* n = dynamic_cast<NodeItem*>(i);
        if(n) {
            names.insert(n->id());
        }
    }
    gw.mGraph->selection(gw.mCurrentMask).clear();
    gw.mGraph->selection(gw.mCurrentMask).add(names);
    gw.updateSelectionColor();
    //gw.group(names);
    gw.setBehaviour(new Behaviour(&gw));
    gw.mScene->removeItem(mRectangle);
    return true;
}

void GraphWidget::updateSelectionColor() {
    Selection& s = mGraph->selection(mCurrentMask);
    for(NodeItem* i : mNodes) {
        if(i->graphicsEffect()) {
            delete i->graphicsEffect();
            i->setGraphicsEffect(nullptr);
        }
        if(s.count(i->id())) {
           QGraphicsColorizeEffect* eff = new QGraphicsColorizeEffect(this);
           eff->setColor(mSelectionColors[mCurrentMask]);
           i->setGraphicsEffect(eff);
        }
    }
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



