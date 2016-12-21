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
    void cmd_up();
    void cmd_down();
    void replace(QString rep);
    void issue();
    void print_prompt();
    void print_error(const QString& error);
    void toBottom();
    ~QConsoleWidget();
private:
    int mFixedPosition;
    int mCmdIndex;
    QStringList mHistory;
protected:
    void keyPressEvent (QKeyEvent * event);
public slots:
    void OnChildStdOutWrite(QString szOutput);
    void cursorPositionChanged();
};

#endif // QCONSOLEWIDGET_H
