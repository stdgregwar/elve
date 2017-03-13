#ifndef EGRAPH_H
#define EGRAPH_H

#include "Graph.h"
#include <array>
#include "interfaces/LayoutPlugin.h"
#include "interfaces/LookFactoryPlugin.h"
#include "Selection.h"
#include "CommandHistory.h"
#include <memory>

namespace Elve {

class ExtendedGraph;

typedef std::shared_ptr<ExtendedGraph> SharedEGraph;
typedef std::array<Selection,10> SelectionMasks;

class GraphWidget;
class ExtendedGraph : public std::enable_shared_from_this<ExtendedGraph>
{
public:
    ExtendedGraph(const SharedGraph& g, const NodePositions& positions = NodePositions());
    static SharedEGraph fromJSON(const QJsonObject& obj);

    static SharedEGraph fromGraph(const SharedGraph& g);

    static SharedEGraph fromFile(const QString& filename);
    void toFile(const QString& filename);

    SharedEGraph group(const NodeIDSet &names, const NodeName &groupName);
    SharedEGraph ungroup(const NodeIDSet &names) const;

    SelectionMasks& selections();
    Selection& selection(size_t i);
    Selection invertedSelection(size_t i) const;
    Selection& currentSelection();

    QJsonObject json() const;
    const NodePositions &positions() const;
    const SharedGraph& graph() const;

    void setLayout(const SharedLayout& l);
    void setLook(const SharedLook& l);

    void setMask(int id);
    int mask() const;
    const SharedLayout& layout();
    const SharedLook& look();

    CommandHistory& history();

    void applyLayout(const NodePositions& p = NodePositions());
    void setView(GraphWidget* view);
    GraphWidget* view();
    SharedEGraph clone(const SharedGraph& graph, const NodePositions& positions = {}) const;
    ~ExtendedGraph();
private:
    mutable bool mPosDirty;
    SharedGraph mGraph;
    mutable NodePositions mPositions; //Defining last saved node positions
    CommandHistory mHistory;
    SharedLayout mLayout;
    SharedLook mLook;
    GraphWidget* mView;
    SelectionMasks mSelections;
    int mMaskId; ///Selection mask index
};

}

#endif // EGRAPH_H
