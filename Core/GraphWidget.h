#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsColorizeEffect>
#include <QGraphicsView>
#include <QPointF>
#include <QComboBox>
#include <QColor>
#include <QPushButton>
#include <QLabel>

#include "System.h"
#include "Graph.h"
#include "ExtendedGraph.h"
#include <interfaces/LayoutPlugin.h>
#include <interfaces/GraphWidgetListener.h>
#include <interfaces/LookFactoryPlugin.h>

namespace Elve {

class MainWindow;

enum SelectionMode {
    CLEAR,
    ADD,
    SUB,
    MULT
};

class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    GraphWidget(QWidget *parent = nullptr, GraphWidgetListener* listener = new DummyListener());

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
    void paintEvent(QPaintEvent* ev) override;

    void group(const Selection &names, const NodeName& groupName = "group");
    void ungroup(const NodeIDSet &names);

    void quickSim(unsigned ticks);
    void setLayout(const SharedLayout &l);
    void reflect(System &sys, SharedGraph g, SharedLook lf);

    void select(const NodeIDSet& names, SelectionMode mode);

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;

    void fit();

    void updateSelectionColor();

    const SharedEGraph& graph() const;
    QGraphicsScene* scene();
    //QJsonObject json() const;
    //void fromJson(const QJsonObject& obj);
    ~GraphWidget();
public slots:
    void borderSelect();
    void toggleSelection();
    void group();
    void ungroup();
    void setCurrentMask(int i);
    void start();
    void stop();
    void toggleSim(bool sim);
    void quickSim();
signals:
    void maskChanged(int i);
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
    void unsetGraph();
    void clearEdgesPaths();
    void flushPen(QPen& pen, QPainterPath& path, const QPen& newPen);
    void updateEdges();

    QGraphicsScene* mScene;
    bool mDrag;
    qreal mScale;
    QPointF mLastPos;
    QGraphicsPixmapItem* mPlayPauseIcon;
    SharedEGraph mGraph;
    Behaviour* mBehaviour;
    QComboBox* mSelectionBox;
    //temp
    std::vector<EdgeLook*> mEdges;
    std::vector<NodeLook*> mNodes;
    QList<QGraphicsPathItem*> mEdgesPaths;
    GraphWidgetListener* mListener;
    static std::array<QColor,10> mSelectionColors;
    qreal mTargetScale;
    int mTimerId;
    bool mSim;
};

}

#endif // GRAPHWIDGET_H
