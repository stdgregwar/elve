#ifndef EGRAPH_H
#define EGRAPH_H

#include "Graph.h"
#include "interfaces/LayoutPlugin.h"

class EGraph;

typedef std::shared_ptr<EGraph> SharedEGraph;

class EGraph
{
public:
    EGraph(const SharedGraph& g, const NodePositions& positions);
    static SharedEGraph fromJSON(const QJsonObject& obj);

    static SharedEGraph fromFile(const QString& filename);
    void toFile(const QString& filename);

    QJsonObject json() const;
    const NodePositions& positions();
    const SharedGraph& graph();
    void setLayout(LayoutPlugin* l);
    LayoutPlugin* layout();
private:
    void savePositions();
    SharedGraph mGraph;
    NodePositions mNodePositions; //Defining last saved node positions
    LayoutPlugin* mLayout;

};

#endif // EGRAPH_H
