#include "DockTab.h"
#include "ui_docktab.h"

DockTab::DockTab(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockTab)
{
    ui->setupUi(this);
}

DockTab::~DockTab()
{
    delete ui;
}
