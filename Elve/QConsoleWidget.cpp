#include "QConsoleWidget.h"
#include "CommandLine.h"
#include <sstream>
#include <QScrollBar>
#include <QDebug>
#include <QKeyEvent>

QConsoleWidget::QConsoleWidget(QWidget *parent) : QTextEdit(parent),
    mHistory{
        "read_graph -n ~/mul5.json"
        },
    mCmdIndex(1)
{
    setUndoRedoEnabled(false);
    setStyleSheet("font : 11pt 'Mono';");
    setWhatsThis(tr("<html><head/><body>"
                    "<p>Elve's Terminal</p>"
                    "<p><br/></p>"
                    "<p>A terminal emulator to issue elve commands. Plugins are automatically exported to the CLI.</p>"
                    "<p>See 'help' command for more info.</p>"
                    "</body></html>"));

    setTextColor(QColor("white"));

    /*QPalette p = this->palette();
    p.setColor(QPalette::Base,QColor(59,58,58));
    p.setColor(QPalette::Text,Qt::white);
    this->setPalette(p);*/
    mFixedPosition = 0;
    print_prompt();
    setLineWrapMode(QConsoleWidget::WidgetWidth);
}

QConsoleWidget::~QConsoleWidget()
{
}


void QConsoleWidget::OnChildStdOutWrite(QString szOutput)
{
#ifdef Q_OS_WIN32
    QString content;

    if (fixedPosition != 0) {
        content = szOutput.right(szOutput.count() - szOutput.indexOf('\n') - 1);
    } else {    
        content = szOutput;
    }

    if (content.startsWith("\f\r\n")) {
        clear();
        insertPlainText(content.right(content.count() - 3));
    } else {
        insertPlainText(content);
    }
#else
    insertPlainText(szOutput);
#endif
    mFixedPosition = textCursor().position();
}

void QConsoleWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->text().size() && textCursor().position() < mFixedPosition){
        toBottom();
    }

    bool accept;
    int key = event->key();
    if (key == Qt::Key_Backspace || event->key() == Qt::Key_Left) {
        accept = textCursor().position() > mFixedPosition;
    } else if (key == Qt::Key_Return) {
        accept = false;
        issue();
    } else if (key == Qt::Key_Up) {
        cmd_up();
        accept = false;
    } else if (key == Qt::Key_Down) {
        cmd_down();
        accept = false;
    } else {
        accept = textCursor().position() >= mFixedPosition;
    }

    if (accept) {
        QTextEdit::keyPressEvent(event);
    }
}

void QConsoleWidget::cmd_up() {
    int oldIndex = mCmdIndex;
    mCmdIndex =std::max(0,mCmdIndex-1);
    if(mCmdIndex != oldIndex) replace(mHistory.at(mCmdIndex));
}

void QConsoleWidget::cmd_down() {
    if(mHistory.isEmpty()) return;
    int oldIndex = mCmdIndex;
    mCmdIndex =std::min(mHistory.size()-1,mCmdIndex+1);
    if(mCmdIndex != oldIndex) replace(mHistory.at(mCmdIndex));
}

void QConsoleWidget::replace(QString rep)
{
    moveCursor(QTextCursor::End);
    QTextCursor c = textCursor();
    c.setPosition(mFixedPosition,QTextCursor::KeepAnchor);
    c.removeSelectedText();
    insertPlainText(rep);
}

void QConsoleWidget::issue() {
    int count = toPlainText().count() - mFixedPosition;
    QString cmd = toPlainText().right(count);
    //Call alice here
    run_command(cmd);
}

void QConsoleWidget::run_command(const QString& cmd, bool print) {
    std::ostringstream stderr;
    std::ostringstream stdout;
    stdout<< std::endl;
    if(print) insertPlainText(cmd);
    CommandLine::get().run_command(cmd,stdout,stderr);
    OnChildStdOutWrite(QString::fromStdString(stdout.str()));
    print_error(QString::fromStdString(stderr.str()));
    print_prompt();

    if(!cmd.isEmpty()) mHistory.append(cmd);
    mCmdIndex = mHistory.size();
}

void QConsoleWidget::toBottom() {
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    moveCursor(QTextCursor::End);
}

void QConsoleWidget::print_prompt() {
    OnChildStdOutWrite("[elve ~]$ ");
    toBottom();
}

void QConsoleWidget::print_error(const QString &error) {
    QColor c = textColor();
    setTextColor(Qt::red);
    OnChildStdOutWrite(error);
    setTextColor(c);
}

void QConsoleWidget::cursorPositionChanged()
{
    if (textCursor().position() < mFixedPosition) {
        textCursor().setPosition(mFixedPosition);
    }
}
