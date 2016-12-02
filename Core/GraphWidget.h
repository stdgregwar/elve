#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QPointF>
#include "System.h"
#include "NodeItem.h"
#include "EdgeItem.h"
#include "Graph.h"
#include "EGraph.h"
#include <interfaces/LayoutPlugin.h>

class MainWindow;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT;
public:
    GraphWidget(QWidget *parent, QString filename = "new file");

    void setGraph(SharedEGraph graph, unsigned quickTicks = 500);
    //void setGraph(SharedGraph graph, const NodePositions& positions);

    void tick(float dt, bool update = true);

    //void drawBackground(QPainter *painter, const QRectF &rect);
    void init();

    //Events
    void timerEvent(QTimerEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void showEvent(QShowEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void group(const NodeIDs& names, const NodeID& groupName = "group");
    void ungroup(const NodeIDs& names);

    void quickSim(unsigned ticks);
    void setLayout(const SharedLayout &l);
    void reflect(System &sys, SharedGraph g);


    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void fit();

    const SharedEGraph& graph() const;
    QJsonObject json() const;
    void fromJson(const QJsonObject& obj);
public slots:
    void borderSelect();
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
    void clearScene();

    QGraphicsScene* mScene;
    bool mDrag;
    qreal mScale;
    QPointF mLastPos;
    SharedEGraph mGraph;
    Behaviour* mBehaviour;
    //LayoutPlugin* mLayout;
    //temp
    std::vector<EdgeItem*> mEdges;
    QGraphicsPathItem* mEdgesPath;
    QString mFilename;
};

#endif // GRAPHWIDGET_H
