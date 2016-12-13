#include "QConsoleWidget.h"
#include "CommandLine.h"
#include <sstream>
#include <QScrollBar>

#include <QKeyEvent>

QConsoleWidget::QConsoleWidget(QWidget *parent) : QTextEdit(parent)
{
    setUndoRedoEnabled(false);
    setStyleSheet("font : 11pt 'Mono';");

    setTextColor(QColor("white"));

    /*QPalette p = this->palette();
    p.setColor(QPalette::Base,QColor(59,58,58));
    p.setColor(QPalette::Text,Qt::white);
    this->setPalette(p);*/
    fixedPosition = 0;
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
    fixedPosition = textCursor().position();
}

void QConsoleWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->text().size() && textCursor().position() < fixedPosition){
        toBottom();
    }

    bool accept;
    int key = event->key();
    if (key == Qt::Key_Backspace || event->key() == Qt::Key_Left) {
        accept = textCursor().position() > fixedPosition;
    } else if (key == Qt::Key_Return) {
        accept = false;
        int count = toPlainText().count() - fixedPosition;
        QString cmd = toPlainText().right(count);
        //Call alice here
        run_command(cmd);
    } else if (key == Qt::Key_Up) {
        accept = false;
    } else {
        accept = textCursor().position() >= fixedPosition;

    }

    if (accept) {
        QTextEdit::keyPressEvent(event);
    }
}

void QConsoleWidget::run_command(const QString& cmd) {
    std::ostringstream stderr;
    std::ostringstream stdout;
    stdout<< std::endl;
    CommandLine::get().run_command(cmd,stdout,stderr);
    OnChildStdOutWrite(QString::fromStdString(stdout.str()));
    print_error(QString::fromStdString(stderr.str()));
    print_prompt();
}

void QConsoleWidget::toBottom() {
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    moveCursor(QTextCursor::End);
}

void QConsoleWidget::print_prompt() {
    OnChildStdOutWrite("elve>");
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
    if (textCursor().position() < fixedPosition) {
        textCursor().setPosition(fixedPosition);
    }
}
