#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QPointF>
#include "System.h"
#include "NodeItem.h"
#include "EdgeItem.h"
#include "Graph.h"
#include "LayoutPolicy.h"

class GraphWidget : public QGraphicsView
{
    Q_OBJECT;
public:
    GraphWidget();
    void setGraph(SharedGraph graph);
    void setGraph(SharedGraph graph, const NodePositions& positions);
    void timerEvent(QTimerEvent *event) override;
    void tick(float dt, bool update = true);
    void wheelEvent(QWheelEvent *event) override;
    //void drawBackground(QPainter *painter, const QRectF &rect);
    void init();
    void showEvent(QShowEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void group(const NodeNames& names, const NodeID& groupName = "group");
    void quickSim(unsigned ticks);
    void setLayout(LayoutPolicy* l);

public slots:
    void borderSelect();
    void setSimpleLayout();
    void setLevelLayout();
private:
    class Behaviour
    {
        public:
            Behaviour(GraphWidget* parent) : gw(*parent) {}
            virtual void onStart(){}
            virtual bool mousePressEvent(QMouseEvent *){return false;}
            virtual bool mouseReleaseEvent(QMouseEvent *){return false;}
            virtual bool mouseMoveEvent(QMouseEvent *){return false;}
            virtual void onEnd(){}
        protected:
            GraphWidget& gw;
    };
    class BorderSelect : public Behaviour
    {
    public:
        BorderSelect(GraphWidget* parent);
        void onStart() override;
        bool mousePressEvent(QMouseEvent *event) override;
        bool mouseReleaseEvent(QMouseEvent *event) override;
        bool mouseMoveEvent(QMouseEvent *event) override;
        void onEnd() override;
    private:
        QPointF mOrigin;
        QGraphicsRectItem* mRectangle;
        QGraphicsPathItem* mCross;
    };

    void setBehaviour(Behaviour* b);
    void clear();

    System mSystem;
    QGraphicsScene* mScene;
    bool mDrag;
    qreal mScale;
    QPointF mLastPos;
    SharedGraph mCurrentGraph;
    Behaviour* mBehaviour;
    LayoutPolicy* mLayout;
};

#endif // GRAPHWIDGET_H
