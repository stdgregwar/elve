#include "EGraph.h"
#include <memory>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "PluginManager.h"
#include <random>

using namespace std;

EGraph::EGraph(const SharedGraph &g, const NodePositions &positions) :
    mGraph(g),
    mPositions(positions),
    mPosDirty(false)
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
                          std::forward_as_tuple(it.key().toStdString()),
                          std::forward_as_tuple(jpos.at(0).toDouble(),jpos.at(1).toDouble()));
    }
    SharedEGraph eg = std::make_shared<EGraph>(g,positions);
    eg->setLayout(PluginManager::get().getLayout(layoutName));
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
            positions.insert(QString::fromStdString(p.first),
                             QJsonArray{pos.x(),pos.y()});
        }
        layout.insert("positions",positions);
        main.insert("layout",layout);
    }
    return main;
}

SharedEGraph EGraph::fromFile(const QString& filename)
{
    QString ext = filename.split(".").last();
    QFile file(filename);
    if(file.open(QFile::ReadOnly)) {
        QJsonDocument doc;
        if(ext.toLower() == "elfe") {
            doc = QJsonDocument::fromBinaryData(file.readAll());
        } else { //Assume it's json
            doc = QJsonDocument::fromJson(file.readAll());
        }
        SharedEGraph eg = fromJSON(doc.object());
        eg->graph()->setFilename(filename.toStdString());
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
        if(suffix == "elfe") {
            file.write(doc.toBinaryData());
        } else {
            file.write(doc.toJson());
        }
        file.close();
    } else {
        throw std::runtime_error("Couldn't write to file " + filename.toStdString());
    }
}

SharedEGraph EGraph::group(const NodeNames& names, const NodeID &groupName) const
{
    mPosDirty = true;
    NodeID trueName = mGraph->uniqueID(groupName);
    NodePositions poss = positions();

    QVector2D groupCenter;
    for(const NodeID& id : names) {
        groupCenter += poss.at(id);
    }
    groupCenter /= names.size();

    poss[trueName] = groupCenter;
    SharedEGraph eg = std::make_shared<EGraph>(mGraph->group(names,trueName),poss);
    eg->setLayout(mLayout->create());
    return eg;
}

SharedEGraph EGraph::ungroup(const NodeNames & names) const
{
    mPosDirty = true;
    NodePositions poss = positions();
    SharedGraph g = mGraph;
    for(const NodeID& name : names) {
        QVector2D base = poss.at(name);
        SharedGraph cg = g->nodes().at(name).getClusteredGraph();
        if(cg) {
            const NodesByID& ugped = cg->nodes();
            static default_random_engine gen;
            for(const NodesByID::value_type& p : ugped) {
                if(!p.second.isInput() && !p.second.isOutput()) {
                    std::uniform_real_distribution<qreal> x(-128,128);
                    std::uniform_real_distribution<qreal> y(-128,128);
                    poss[p.first] = base + QVector2D(x(gen),y(gen));
                }
            }
        }
        g = g->ungroup(name);
    }
    SharedEGraph eg =std::make_shared<EGraph>(g,poss);
    eg->setLayout(mLayout->create());
    return eg;
}

EGraph::~EGraph()
{
}

void EGraph::applyLayout(const NodePositions &p) {
    if(mLayout) {
        mLayout->setGraph(mGraph,p);
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

const SharedGraph& EGraph::graph() const
{
    return mGraph;
}

void EGraph::setLayout(const SharedLayout &l)
{
    //mPosDirty = true;
    l->setGraph(mGraph,positions());
    mLayout = l;
}

const SharedLayout &EGraph::layout()
{
    return mLayout;
}
