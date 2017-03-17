#include "Dialog.h"
#include "ui_dialog.h"
#include <interfaces/GraphWidgetListener.h>

Dialog::Dialog(LevelLayout *l, Elve::GraphWidgetListener *listener) :
    QDialog(),
    ui(new Ui::Dialog),
    mL(l),
    mListener(listener)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(onAccept()));
}

void Dialog::onAccept() {
    QString command = QString("%1_layout -k %2 -l %3 -d %4 -u %5 -o %6 -r %7 %8")
            .arg(mL->cliName().c_str())
            .arg(QString::number(ui->kSpinBox->value()))
            .arg(QString::number(ui->springSpinBox->value()))
            .arg(QString::number(ui->dampSpinBox->value()))
            .arg(QString::number(ui->unitSpinBox->value()))
            .arg(QString::number(ui->IOUnitSpinBox->value()))
            .arg(QString::number(ui->repSpinBox->value()))
            .arg(ui->interleave->checkState() == Qt::Checked ? "-i" : "");
    mListener->runCommand(command);
}

Dialog::~Dialog()
{
    delete ui;
}
