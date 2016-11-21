#ifndef EGRAPH_H
#define EGRAPH_H

#include "Graph.h"
#include "interfaces/LayoutPlugin.h"

class EGraph;

typedef std::shared_ptr<EGraph> SharedEGraph;

class EGraph
{
public:
    EGraph(const SharedGraph& g, const NodePositions& positions = NodePositions());
    static SharedEGraph fromJSON(const QJsonObject& obj);

    static SharedEGraph fromGraph(const SharedGraph& g);

    static SharedEGraph fromFile(const QString& filename);
    void toFile(const QString& filename);

    SharedEGraph group(const NodeNames& names, const NodeID &groupName) const;
    SharedEGraph ungroup(const NodeNames & names) const;

    QJsonObject json() const;
    const NodePositions &positions() const;
    const SharedGraph& graph() const;
    void setLayout(LayoutPlugin* l);
    LayoutPlugin* layout();
    void applyLayout(const NodePositions& p = NodePositions());
    ~EGraph();
private:
    mutable bool mPosDirty;
    SharedGraph mGraph;
    mutable NodePositions mPositions; //Defining last saved node positions
    LayoutPlugin* mLayout;
};

#endif // EGRAPH_H
