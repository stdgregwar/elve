#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "LevelLayout.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(LevelLayout* l, Elve::GraphWidgetListener* listener);
    ~Dialog();
public slots:
    void onAccept();
private:
    Ui::Dialog *ui;
    Elve::GraphWidgetListener* mListener;
    LevelLayout* mL;
};

#endif // DIALOG_H
