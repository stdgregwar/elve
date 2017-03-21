#ifndef NODELOOK_H
#define NODELOOK_H

#include <QGraphicsItemGroup>
#include <Node.h>
#include <Movable.h>

namespace Elve {

/**
 * @brief Orientation of the look
 */
enum OrientationHint {
    BOTTOMUP, ///Inputs at bottom outputs top
    TOPDOWN, ///inputs top outputs down
    LEFTRIGHT, ///Layouts from left to right
    RIGHTLEFT ///Layouts from right to left
};

/**
 * @brief The NodeLook abstract class
 *
 * This abstract class represent the appeareance of a graph's node in the viewport
 * and some of its basic properties.
 */
class NodeLook : public QGraphicsItemGroup, public Movable
{
public:

    /**
     * @brief NodeLook
     * @param n the node to make look for
     */
    NodeLook(const Node& n);

    //To override when creating looks

    /**
     * @brief position of the input at index
     * @param index index of the input
     * @return a point in the scene
     *
     * Re-implement this method to tell your EdgeLook instances where are their ends
     */
    virtual QPointF inputPos(int index) const ;

    /**
     * @brief position of the output at index
     * @param index index of the output
     * @return a point in the scene
     *
     * Re-implement this method to tell your EdgeLook instances where are their ends
     */
    virtual QPointF outputPos(int index) const ;

    /**
     * @brief called when the selection color of the node changes
     * @param col
     */
    virtual void onColorChange(const QColor& col) = 0;

    /**
     * @brief called when the node is unselected and must be reverted to normal
     * colors
     */
    virtual void onColorReset() = 0;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void onStateChange(const QVector2D& pos, const QVector2D& speed) override;

    /**
     * @brief set the selection color to color
     * @param color the color
     */
    void color(const QColor& color);

    /**
     * @brief get the selection color
     * @return the selection color
     */
    const QColor& color() const;

    /**
     * @brief get the node this Look is for
     * @return const ref to the node
     */
    const Node& node() const;
private:
    const Node& mNode;
    QPointF mOffset;
    QColor  mSelectionColor;
    bool mDraged;
};
}
#endif // NODELOOK_H
