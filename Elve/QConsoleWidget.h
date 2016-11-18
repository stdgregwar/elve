#ifndef QCONSOLEWIDGET_H
#define QCONSOLEWIDGET_H

#include <QTextEdit>
#include <QDir>

class QConsoleWidget : public QTextEdit
{
    Q_OBJECT
public:
    QConsoleWidget(QWidget *parent = 0);
    ~QConsoleWidget();
private:
    int fixedPosition;
protected:
    void keyPressEvent (QKeyEvent * event);
public slots:
    void OnChildStdOutWrite(QString szOutput);
    void cursorPositionChanged();
};

#endif // QCONSOLEWIDGET_H
