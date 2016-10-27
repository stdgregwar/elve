#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QGraphicsPathItem>
#include <QVector2D>
#include <vector>
#include "Movable.h"


class EdgeItem : public QGraphicsPathItem
{
public:
    EdgeItem(size_t segments = 3);
    void setHandleState(size_t i, const QVector2D& pos, const QVector2D& speed = {0,0});
    Movable* getHandle(size_t i);
    size_t segments() const;
private:
    class Handle : public Movable {
    public:
        Handle(EdgeItem* e) : mE(e) {}
        void onStateChange(const QVector2D& pos, const QVector2D& speed)
        {
            mPos = pos;
            mE->handleMoved();
        }

        const QVector2D& pos() const{return mPos;}
        QVector2D& pos() {return mPos;}
    private:
        QVector2D mPos;
        EdgeItem* mE;
    };

    void doPath();
    void handleMoved();
    std::vector<Handle> mHandles;
    unsigned mMoveCount;
};

#endif // EDGEITEM_H
