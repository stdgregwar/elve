#ifndef EGRAPH_H
#define EGRAPH_H

#include "Graph.h"
#include <array>
#include "interfaces/LayoutPlugin.h"
#include "Selection.h"

class EGraph;

typedef std::shared_ptr<EGraph> SharedEGraph;
typedef std::array<Selection,10> SelectionMasks;

class GraphWidget;
class EGraph
{
public:
    EGraph(const SharedGraph& g, const NodePositions& positions = NodePositions());
    static SharedEGraph fromJSON(const QJsonObject& obj);

    static SharedEGraph fromGraph(const SharedGraph& g);

    static SharedEGraph fromFile(const QString& filename);
    void toFile(const QString& filename);

    SharedEGraph group(const NodeIDSet &names, const NodeName &groupName) const;
    SharedEGraph ungroup(const NodeIDs & names) const;

    SelectionMasks& selections();
    Selection& selection(size_t i);

    QJsonObject json() const;
    const NodePositions &positions() const;
    const SharedGraph& graph() const;

    void setLayout(const SharedLayout& l);
    const SharedLayout& layout();
    void applyLayout(const NodePositions& p = NodePositions());
    void setView(GraphWidget* view);
    GraphWidget* view();
    ~EGraph();
private:
    mutable bool mPosDirty;
    SharedGraph mGraph;
    mutable NodePositions mPositions; //Defining last saved node positions
    SharedLayout mLayout;
    GraphWidget* mView;
    SelectionMasks mSelections;
};

#endif // EGRAPH_H
