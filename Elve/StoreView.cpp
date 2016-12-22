#include "StoreView.h"
#include <QDebug>
#include "MainWindow.h"

StoreView::StoreView(Store& store, MainWindow *parent) : QListWidget(parent), mStore(store), mWindow(parent)
{
    mStore.set_callback([this]{update();});
    setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(setCurrentStoreIndex(QModelIndex)));
}

void StoreView::update() {
    clear();
    int i = 0;
    int current = mStore.current_index();
    for(SharedEGraph eg : mStore.data()) {
        if(eg) {
            addItem(QString("%1 %2").arg(QString::number(i++),eg->graph()->filename().split("/").last()));
        } else {
            addItem(QString("%1 Empty").arg(QString::number(i++)));
        }
        //item(i-1)->setSelected(i-1 == current);
    }
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
