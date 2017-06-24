#ifndef QCONSOLEWIDGET_H
#define QCONSOLEWIDGET_H

#include <QTextEdit>
#include <QDir>
#include <QSocketNotifier>


namespace Elve {

class QConsoleWidget : public QTextEdit
{
    Q_OBJECT
public:
    QConsoleWidget(QWidget *parent = 0);
    void run_command(const QString& cmd,bool print = false);
    void cmd_up();
    void cmd_down();
    void replace(QString rep);
    void issue();
    void print_prompt();
    void print_error(const QString& error);
    void toBottom();
    ~QConsoleWidget();
private:
    QSocketNotifier* mNotifier;
    int mFixedPosition;
    int mCmdIndex;
    QStringList mHistory;
protected:
    void keyPressEvent (QKeyEvent * event);
public slots:
    void OnChildStdOutWrite(QString szOutput);
    void cursorPositionChanged();
    void readStdin();
};

}

#endif // QCONSOLEWIDGET_H
