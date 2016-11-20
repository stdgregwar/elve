#include "EGraph.h"
#include <memory>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "PluginManager.h"

EGraph::EGraph(const SharedGraph &g, const NodePositions &positions) : mGraph(g),mNodePositions(positions)
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
    SharedEGraph eg =  std::make_shared<EGraph>(g,positions);
    eg->setLayout(PluginManager::get().getLayout(layoutName));
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
        return fromJSON(doc.object());
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

const NodePositions& EGraph::positions()
{
    return mNodePositions;
}

const SharedGraph& EGraph::graph()
{
    return mGraph;
}

void EGraph::setLayout(LayoutPlugin* l)
{
    mLayout = l;
}

LayoutPlugin* EGraph::layout()
{
    return mLayout;
}

void EGraph::savePositions()
{
    mNodePositions = mLayout->system().positions();
}
