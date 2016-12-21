#ifndef STOREVIEW_H
#define STOREVIEW_H

#include <QListWidget>
#include "CommandLine.h"

class StoreView : public QListWidget
{
public:
    StoreView(Store &store, QWidget* parent = nullptr);
    void update();
public slots:
private:
    Store& mStore;
};

#endif // STOREVIEW_H
