#ifndef STOREVIEW_H
#define STOREVIEW_H

#include <QListWidget>
#include "CommandLine.h"
class MainWindow;
class StoreView : public QListWidget
{
    Q_OBJECT
public:
    StoreView(Store &store, MainWindow* parent = nullptr);
    void update(Store::update u);
public slots:
    void setCurrentStoreIndex(QModelIndex);
    void showStoreIndex(QModelIndex);
private:
    Store& mStore;
    MainWindow* mWindow;
};

#endif // STOREVIEW_H
