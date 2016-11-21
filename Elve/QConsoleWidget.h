#ifndef QCONSOLEWIDGET_H
#define QCONSOLEWIDGET_H

#include <QTextEdit>
#include <QDir>

class QConsoleWidget : public QTextEdit
{
    Q_OBJECT
public:
    QConsoleWidget(QWidget *parent = 0);
    void run_command(const QString& cmd);
    void print_prompt();
    void print_error(const QString& error);
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
