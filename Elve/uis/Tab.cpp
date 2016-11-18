#include "Tab.h"
#include "ui_tab.h"

Tab::Tab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab)
{
    ui->setupUi(this);
}

Tab::~Tab()
{
    delete ui;
}
