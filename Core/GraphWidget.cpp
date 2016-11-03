#include "GraphWidget.h"

#include <QTimerEvent>
#include <QDebug>
#include <cmath>
#include <QWheelEvent>
#include <QOpenGLWidget>

#include <unordered_map>
#include <random>
#include <chrono>

#include "SimpleForceLayout.h"
#include "LevelForceLayout.h"

#define SS 32000

using namespace std;

GraphWidget::GraphWidget() : mScene(new QGraphicsScene(-SS,-SS,SS*2,SS*2,this)) , mDrag(false), mScale(1), mBehaviour(new Behaviour(this)), mLayout(nullptr)
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setGeometry(0,0,1280,720);
    setScene(mScene);
    setBackgroundBrush(QBrush(QColor(59,58,58), Qt::SolidPattern));
    startTimer(1000/60);
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
    mLayout->clear();
}

void GraphWidget::setGraph(SharedGraph graph) {
    if(mLayout) {
        mLayout->setGraph(graph);
        reflect(mLayout->system(),graph);
    }
    mCurrentGraph = graph;
}

void GraphWidget::setGraph(SharedGraph graph, const NodePositions& positions) {
    if(mLayout) {
        mLayout->setGraph(graph,positions);
        reflect(mLayout->system(),graph);
    }
    mCurrentGraph = graph;
}

void GraphWidget::quickSim(unsigned ticks)
{
    mLayout->quickSim(ticks);
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

void GraphWidget::group(const NodeNames &names, const NodeID &groupName) {
    NodeID trueName = mCurrentGraph->uniqueID(groupName);
    NodePositions positions = mLayout->system().positions();

    QVector2D groupCenter;
    for(const NodeID& id : names) {
        groupCenter += positions.at(id);
    }
    groupCenter /= names.size();

    positions[trueName] = groupCenter;
    setGraph(mCurrentGraph->group(names,trueName), positions);
}

void GraphWidget::tick(float dt, bool update)
{
    if(mLayout) {
        mLayout->tick(dt,update);
    }
}

void GraphWidget::timerEvent(QTimerEvent *e)
{
    tick(0.1);
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
    NodePositions old_pos;
    if(mLayout) {
        old_pos = mLayout->system().positions();
        //delete mLayout;
        mLayout = l;
        if(mCurrentGraph) {
            setGraph(mCurrentGraph,old_pos);
        }
    } else {
        mLayout = l;
        if(mCurrentGraph) {
            setGraph(mCurrentGraph);
        }
    }

}

void GraphWidget::reflect(System &sys,SharedGraph g) {
    mScene->clear();
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
            mScene->addItem(ei);
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
    QList<QGraphicsItem*> items = gw.mScene->items(mRectangle->boundingRect());
    NodeNames names;
    for(QGraphicsItem* i : items) {
        NodeItem* n = dynamic_cast<NodeItem*>(i);

        if(n) {
            //n->setBrush(QBrush(QColor(240,0,0)));
            names.insert(n->id());
        }
    }
    gw.group(names);
    gw.setBehaviour(new Behaviour(&gw));
    return true;
}

bool GraphWidget::BorderSelect::mouseMoveEvent(QMouseEvent *event) {

    float inf = 800000; //TODO cross
    QPointF pos = gw.mapToScene(event->pos());
    if(mCross) {

    } else {
        QPointF origin = mRectangle->pos();
        mRectangle->setRect(origin.x(),origin.y(),
                            (pos.x()-origin.x()),(pos.y()-origin.y()));
    }
    return true;
}

void GraphWidget::BorderSelect::onEnd() {
    if(mCross) {
        gw.mScene->removeItem(mCross);
        delete mCross;
    }
    //gw.mScene->removeItem(mRectangle);
    //delete mRectangle;
}



