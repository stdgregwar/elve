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

/**
 * @brief Extended graph representation with layout,look,selection....
 *
 * The extended graph embbeds the Graph to add Visualisation specific properties
 * to it.
 */
class ExtendedGraph : public std::enable_shared_from_this<ExtendedGraph>
{
public:
    /**
     * @brief ExtendedGraph from graph and initial nodes positions
     * @param g underlying Graph
     * @param positions initial node positions
     */
    ExtendedGraph(const SharedGraph& g, const NodePositions& positions = NodePositions());

    /**
     * @brief construct an ExtendedGraph from a json field
     * @param obj the json object
     * @return a new shared Extended graph
     */
    static SharedEGraph fromJSON(const QJsonObject& obj);

    /**
     * @brief create extended graph from a abstract Graph
     * @param g the Graph
     * @return
     */
    static SharedEGraph fromGraph(const SharedGraph& g);

    /**
     * @brief create and ExtendedGraph from a file
     * @param filename filepath
     * @return new shared ExtendedGraph
     */
    static SharedEGraph fromFile(const QString& filename);

    /**
     * @brief write extended Graph to file
     * @param filename
     */
    void toFile(const QString& filename);

    /**
     * @brief similar to Graph::group but taking care of nodes positions
     * @param names Node ids
     * @param groupName name of the group
     * @return new shared ExtendedGraph
     */
    SharedEGraph group(const NodeIDSet &names, const NodeName &groupName);

    /**
     * @brief similar to Graph::ungroup but taking care of nodes positions
     * @param names Node ids set
     * @return new shared ExtendedGraph
     */
    SharedEGraph ungroup(const NodeIDSet &names) const;

    /**
     * @brief access selection masks
     * @return
     */
    SelectionMasks& selections();

    /**
     * @brief access ith selection mask
     * @param i index between 0 and 9
     * @return selection
     */
    Selection& selection(size_t i);

    /**
     * @brief get the inverted ith selection mask
     * @param i index between 0 and 9
     * @return
     */
    Selection invertedSelection(size_t i) const;

    /**
     * @brief get current selection mask
     * @return
     */
    Selection& currentSelection();

    /**
     * @brief construct json representation of the ExtendedGraph
     * @return a json containing all the graph
     */
    QJsonObject json() const;

    /**
     * @brief all the positions of the nodes
     * @return
     */
    const NodePositions &positions() const;

    /**
     * @brief the underlying abstract Graph
     * @return
     */
    const SharedGraph& graph() const;

    /**
     * @brief set the layout for this graph
     * @param l a Valid shared Layout
     */
    void setLayout(const SharedLayout& l);

    /**
     * @brief set the look for this graph
     * @param l a valid shared Look
     */
    void setLook(const SharedLook& l);

    /**
     * @brief set id of current selection mask
     * @param id index between 0 and 9
     */
    void setMask(int id);

    /**
     * @brief get id of current selection mask
     * @return  current selection id
     */
    int mask() const;

    /**
     * @brief get shared layout
     * @return
     */
    const SharedLayout& layout();

    /**
     * @brief look
     * @return
     */
    const SharedLook& look();

    /**
     * @brief access command history of this graph
     * @return
     */
    CommandHistory& history();

    /**
     * @brief used internally to trigger layout application
     * @param p
     */
    void applyLayout(const NodePositions& p = NodePositions());

    /**
     * @brief used internally to acknoledge graph that it's being shown
     * @param view
     */
    void setView(GraphWidget* view);

    /**
     * @brief get the graph viewport
     * @return
     */
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
