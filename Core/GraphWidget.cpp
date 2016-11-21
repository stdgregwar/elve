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

GraphWidget::GraphWidget(QWidget* parent, QString filename) : QGraphicsView(parent),
    mScene(new QGraphicsScene(-SS,-SS,SS*2,SS*2,this)),
    mDrag(false),
    mScale(1),
    mBehaviour(new Behaviour(this)),
    mEdgesPath(new QGraphicsPathItem()),
    mFilename(filename)
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
    if(graph->layout()) {
        graph->applyLayout();
        reflect(graph->layout()->system(),graph->graph());
        quickSim(quickTicks);
    }
}

/*void GraphWidget::setGraph(SharedGraph graph, const NodePositions& positions) {
    if(mLayout) {
        mLayout->setGraph(graph,positions);
        reflect(mLayout->system(),graph);
    }
    mCurrentGraph = graph;
}*/

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

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    if(event->key() == Qt::Key_Q) {
        if(mGraph->layout()) {
            mGraph->layout()->quickSim(400);
        }
    }
}

void GraphWidget::ungroup(const NodeNames& names) {
    setGraph(mGraph->ungroup(names));
}

void GraphWidget::group(const NodeNames &names, const NodeID &groupName) {
    setGraph(mGraph->group(names,groupName));
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

void GraphWidget::setLayout(LayoutPlugin *l) {
    mGraph->setLayout(l);
    reflect(mGraph->layout()->system(),mGraph->graph());
}

void GraphWidget::clearScene() {
    mScene->removeItem(mEdgesPath);
    mScene->clear();
    mScene->addItem(mEdgesPath);
    for_each(mEdges.begin(),mEdges.end(),[](EdgeItem* e){delete e;});
    mEdges.clear();
}

void GraphWidget::reflect(System &sys,SharedGraph g) {
    clearScene();
    for(auto& nbi : g->nodes()) {
        const Node& n = nbi.second;
        NodeItem* ni = new NodeItem(n.id(),n.type());
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
    QPen dashPen = QPen(Qt::blue,Qt::DashLine);
    mRectangle = gw.mScene->addRect({0,0,0,0},
                                    dashPen);
    mCross = gw.mScene->addPath(QPainterPath(),dashPen);
}

bool GraphWidget::BorderSelect::mousePressEvent(QMouseEvent *event) {
    gw.mScene->removeItem(mCross);
    delete mCross;
    mCross = nullptr;
    mRectangle->setPos(gw.mapToScene(event->pos()));
    return true;
}

bool GraphWidget::BorderSelect::mouseReleaseEvent(QMouseEvent *event) {
    QList<QGraphicsItem*> items = gw.mScene->items(mRectangle->sceneBoundingRect());
    NodeNames names;
    for(QGraphicsItem* i : items) {
        NodeItem* n = dynamic_cast<NodeItem*>(i);

        if(n) {
            //n->setBrush(QBrush(QColor(240,0,0)));
            names.insert(n->id());
        }
    }
    gw.ungroup(names);
    gw.setBehaviour(new Behaviour(&gw));
    return true;
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
        delete mCross;
    }
    //gw.mScene->removeItem(mRectangle);
    //delete mRectangle;
}



