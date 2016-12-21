#include "StoreView.h"
#include <QDebug>

StoreView::StoreView(Store& store, QWidget *parent) : QListWidget(parent), mStore(store)
{
    mStore.set_callback([this]{update();});
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void StoreView::update() {
    clear();
    int i = 0;
    for(SharedEGraph eg : mStore.data()) {
        if(eg) {
            addItem(QString("%1 %2").arg(QString::number(i++),eg->graph()->filename().split("/").last()));
        } else {
            addItem(QString("%1 Empty").arg(QString::number(i++)));
        }
    }
    qDebug() << "Current " << mStore.current_index();

    item(mStore.current_index())->setSelected(true);
}
