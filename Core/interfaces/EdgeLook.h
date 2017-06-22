#ifndef EDGELOOK_H
#define EDGELOOK_H

#include "NodeLook.h"
namespace Elve {

/**
 * @brief Class representing look of an edge
 *
 * The EdgeLook class is an abstract class that must be reimplemented in the
 * look plugins to provide information about the look of an edge and its
 * geometry.
 */
class EdgeLook
{
public:
    /**
     * @brief EdgeLook only constructor
     * @param from the Node(Look) from which the edge comes
     * @param outi the output index from which the edge comes
     * @param to the Node(Look) to which the edge goes
     * @param ini the input index to which the edge goes
     */
    EdgeLook(const NodeLook& from, Index outi ,const NodeLook& to, Index ini);

    /**
     * @brief add the shape of this edge
     * @param path path to be updated
     *
     * The edges in elve are batched together to speed up drawing, this is thus
     * the only way to add geometry from the edge, update the provided path by
     * adding the current edge to it. Don't clear the given path.
     */
    virtual void addToPath(QPainterPath& path) = 0;

    /**
     * @brief state the pen that must draw this edge
     * @return the pen
     *
     * Since edges are batched in elve, and qt can draw only with one pen at
     * once. Edges are sorted by Pen values to group drawing of edges sharing
     * common pens.
     */
    virtual QPen pen() const = 0;

    /**
     * @brief get the node from which the edge is coming
     * @return
     */
    const NodeLook& from() const;

    /**
     * @brief get the node to which the edge is going
     * @return
     */
    const NodeLook& to() const;

    /**
     * @brief get the output index (pin) from which the edge is coming
     * @return
     */
    Index iFrom() const;

    /**
     * @brief get the input index (pin) to which the edge is going
     * @return
     */
    Index iTo() const;
private:
    const NodeLook& mFrom;
    const NodeLook& mTo;
    Index mIFrom;
    Index mITo;
};
}
#endif // EDGELOOK_H
