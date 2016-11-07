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

#include "SimpleForceLayout.h"
#include "LevelForceLayout.h"

#define SS 32000

using namespace std;

GraphWidget::GraphWidget() : mScene(new QGraphicsScene(-SS,-SS,SS*2,SS*2,this)),
    mDrag(false),
    mScale(1),
    mBehaviour(new Behaviour(this)),
    mLayout(nullptr),
    mEdgesPath(new QGraphicsPathItem())
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
    mLayout->clear();
    mEdges.clear();
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

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect){
    QGraphicsView::drawBackground(painter,rect);
    if(mLayout) {
        mLayout->system().debug(painter);
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
        if(mLayout) {
            mLayout->quickSim(400);
        }
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
        QPainterPath p;
        std::for_each(mEdges.begin(),mEdges.end(),[&p](EdgeItem*& e){
            e->doPath(p);
        });
        mEdgesPath->setPath(p);
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
        mRectangle->setRect(0,0,
                            (pos.x()-origin.x()),(pos.y()-origin.y()));
    }
    return true;
}

QJsonObject GraphWidget::json() const {
    if(!mCurrentGraph) {
        return QJsonObject();
    }

    QJsonObject main;
    main.insert("graph",mCurrentGraph->json());

    if(mLayout) {
        QJsonObject layout;
        QJsonObject  positions;

        layout.insert("name",mLayout->layoutName());

        using pair_type = NodePositions::value_type;
        for(const pair_type& p : mLayout->system().positions()) {
            const QVector2D& pos = p.second;
            positions.insert(QString::fromStdString(p.first),
                             QJsonArray{pos.x(),pos.y()});
        }
        layout.insert("positions",positions);
        main.insert("layout",layout);
    }
    return main;
}

void GraphWidget::fromJson(const QJsonObject& obj)
{
    SharedGraph g = Graph::fromJson(obj.value("graph").toObject());
    //positions
    NodePositions positions; positions.reserve(g->nodeCount());
    QJsonObject jpositions = obj["layout"].toObject()["positions"].toObject();
    for(QJsonObject::const_iterator it = jpositions.constBegin();
        it != jpositions.constEnd();
        it++) {

        QJsonArray jpos = it.value().toArray();
        positions.emplace(std::piecewise_construct,
                          std::forward_as_tuple(it.key().toStdString()),
                          std::forward_as_tuple(jpos.at(0).toDouble(),jpos.at(1).toDouble()));
    }
    //TODO load layout type etc....
    setGraph(g,positions);
}

const SharedGraph GraphWidget::graph() const
{
    return mCurrentGraph;
}

void GraphWidget::BorderSelect::onEnd() {
    if(mCross) {
        gw.mScene->removeItem(mCross);
        delete mCross;
    }
    //gw.mScene->removeItem(mRectangle);
    //delete mRectangle;
}



