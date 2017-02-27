#include "EGraph.h"
#include <memory>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "PluginManager.h"
#include <random>
#include "GraphWidget.h"

using namespace std;

EGraph::EGraph(const SharedGraph &g, const NodePositions &positions) :
    mGraph(g),
    mPositions(positions),
    mPosDirty(false),
    mView(nullptr),
    mMaskId(1)
{

}

SharedEGraph EGraph::fromJSON(const QJsonObject &obj)
{
    SharedGraph g = Graph::fromJson(obj.value("graph").toObject());
    //positions
    NodePositions positions; positions.reserve(g->nodeCount());
    QJsonObject jpositions = obj["layout"].toObject()["positions"].toObject();
    QString layoutName = obj["layout"].toObject()["name"].toString();
    for(QJsonObject::const_iterator it = jpositions.constBegin();
        it != jpositions.constEnd();
        it++) {

        QJsonArray jpos = it.value().toArray();
        positions.emplace(std::piecewise_construct,
                          std::forward_as_tuple(it.key().toInt()),
                          std::forward_as_tuple(jpos.at(0).toDouble(),jpos.at(1).toDouble()));
    }

    QJsonObject selections = obj["selections"].toObject();

    int i = 0;
    SelectionMasks masks;
    for(Selection& s : masks) {
        s = Selection::fromJson(selections.value(QString::number(i++)).toArray());
    }
    SharedEGraph eg = std::make_shared<EGraph>(g,positions);
    eg->selections() = masks;
    eg->setMask(obj["mask"].toInt());
    eg->setLayout(PluginManager::get().getLayout(layoutName));
    eg->setLook(PluginManager::get().defaultLook());
    return eg;
}

SharedEGraph EGraph::fromGraph(const SharedGraph& g)
{
    SharedEGraph eg =  std::make_shared<EGraph>(g);
    eg->setLayout(PluginManager::get().defaultLayout());
    return eg;
}

QJsonObject EGraph::json() const
{
    if(!mGraph) {
        return QJsonObject();
    }

    QJsonObject main;
    main.insert("graph",mGraph->json());

    if(mLayout) {
        QJsonObject layout;
        QJsonObject  positions;

        layout.insert("name",mLayout->name());

        using pair_type = NodePositions::value_type;
        for(const pair_type& p : mLayout->system().positions()) {
            const QVector2D& pos = p.second;
            positions.insert(to_string(p.first).c_str(),
                             QJsonArray{pos.x(),pos.y()});
        }
        layout.insert("positions",positions);
        main.insert("layout",layout);
    }

    main.insert("mask",mMaskId);

    QJsonObject selections;
    int i = 0;
    for(const Selection& s : mSelections) {
        selections.insert(QString::number(i++),s.json());
    }
    main.insert("selections",selections);
    return main;
}

SharedEGraph EGraph::fromFile(const QString& filename)
{
    QString ext = filename.split(".").last();
    QFile file(filename);
    if(file.open(QFile::ReadOnly)) {
        QJsonDocument doc;
        if(ext.toLower() == "elve") {
            doc = QJsonDocument::fromBinaryData(file.readAll());
        } else { //Assume it's json
            doc = QJsonDocument::fromJson(file.readAll());
        }
        SharedEGraph eg = fromJSON(doc.object());
        //eg->graph()->setFilename(filename.toStdString());
        return eg;
    } else {
        throw std::runtime_error("Couldn't open file " + filename.toStdString() + " for reading.");
    }
}

void EGraph::toFile(const QString& filename) {
    QString suffix = filename.split(".").last();
    QJsonDocument doc(json());

    QFile file(filename);
    if(file.open(QFile::WriteOnly)) {
        if(suffix == "elve") {
            file.write(doc.toBinaryData());
        } else {
            file.write(doc.toJson());
        }
        file.close();
    } else {
        throw std::runtime_error("Couldn't write to file " + filename.toStdString());
    }
}

SharedEGraph EGraph::group(const NodeIDSet& names, const NodeName &groupName)
{
    if(names.size() == 0) return shared_from_this();
    mPosDirty = true;
    NodePositions poss = positions();
    NodeID nid = mGraph->newID();
    QVector2D groupCenter;
    for(const NodeID& id : names) {
        groupCenter += poss.at(id);
    }
    groupCenter /= names.size();

    poss[nid] = groupCenter;
    SharedEGraph eg = clone(mGraph->group(names,nid,groupName),poss);

    for(int i = 0; i < 10; i++) {
        for(const NodeID& id : selection(i)) {
            eg->selection(i).add(eg->graph()->alias(i).id);
        }
    }
    return eg;
}

SharedEGraph EGraph::ungroup(const NodeIDSet & names) const
{
    mPosDirty = true;
    NodePositions poss = positions();
    SharedGraph g = mGraph;
    for(const NodeID& name : names) {
        QVector2D base = poss.at(name);
        const Dependencies& ids = g->data(name).dependencies();
        static qreal radius = 128;
        for(size_t i = 0; i < ids.size(); ++i) {
            qreal p = M_PI*((qreal)i)/ids.size();
            poss[ids[i].id] = base + QVector2D(radius*cos(p),radius*sin(p));
        }
        g = g->ungroup(name);
    }
    return clone(g,poss);
}

SharedEGraph EGraph::clone(const SharedGraph& graph, const NodePositions& positions) const {
    SharedEGraph eg =std::make_shared<EGraph>(graph,positions);
    eg->setLook(mLook);
    eg->setLayout(mLayout->create());
    eg->setView(mView);
    return eg;
}

SelectionMasks& EGraph::selections() {
    return mSelections;
}

Selection& EGraph::selection(size_t i) {
    return mSelections[i];
}


EGraph::~EGraph()
{
}

void EGraph::applyLayout(const NodePositions &p) {
    if(mLayout) {
        mLayout->setGraph(shared_from_this(),p);
    }
}

const NodePositions& EGraph::positions() const
{
    if(mPosDirty && mLayout) {
        mPositions = mLayout->system().positions();

    }
    mPosDirty = false;
    return mPositions;
}

void EGraph::setView(GraphWidget *view) {
    mView = view;
}

GraphWidget* EGraph::view() {
    return mView;
}

const SharedGraph& EGraph::graph() const
{
    return mGraph;
}

void EGraph::setMask(int id) {
    mMaskId = id;
}

int EGraph::mask() const {
    return mMaskId;
}

Selection& EGraph::currentSelection() {
    return selection(mMaskId);
}

void EGraph::setLayout(const SharedLayout &l)
{
    if(!l) return;
    mPosDirty = true;
    if(mLook) l->setGraph(shared_from_this(),positions());
    mLayout = l;
    if(mView && mLook) mView->reflect(l->system(),mGraph,mLook);
}

void EGraph::setLook(const SharedLook& l) {
    mLook = l;
    if(mLayout) mLayout->setGraph(shared_from_this(),positions());
    if(mView && mLayout) mView->reflect(mLayout->system(),mGraph,mLook);
}

const SharedLayout &EGraph::layout()
{
    return mLayout;
}

const SharedLook& EGraph::look()
{
    return mLook;
}
