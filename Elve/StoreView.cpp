#include "StoreView.h"
#include <QDebug>
#include "MainWindow.h"

StoreView::StoreView(Store& store, MainWindow *parent) : QListWidget(parent), mStore(store), mWindow(parent)
{
    mStore.set_callback([this](Store::update u){update(u);});
    setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(setCurrentStoreIndex(QModelIndex)));
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showStoreIndex(QModelIndex)));
}

void StoreView::update(Store::update u) {
    if(u == Store::ALL) {
        clear();
        int i = 0;
        for(SharedEGraph eg : mStore.data()) {
            if(eg) {
                addItem(QString("%1 %2").arg(QString::number(i++),eg->graph()->filename().split("/").last()));
            } else {
                addItem(QString("%1 Empty").arg(QString::number(i++)));
            }
        }
    }
    int current = mStore.current_index();
    setCurrentRow(current);
}

void StoreView::setCurrentStoreIndex(QModelIndex qmi) {
    int index = qmi.row();
    if(index != -1 && index >= 0) {
        mWindow->runUiCommand(QString("current -g -i %1").arg(QString::number(index)));
    } else {
        //update();
    }
}

void StoreView::showStoreIndex(QModelIndex qmi) {
    int index = qmi.row();
    if(index == mStore.current_index()) {
        mWindow->runUiCommand(QString("show -g"));
    }
}
