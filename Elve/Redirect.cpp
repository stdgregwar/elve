// Redirect.cpp : implementation file
//

#include "Redirect.h"

/////////////////////////////////////////////////////////////////////////////
// CRedirect class

CRedirect::CRedirect()
{
}

CRedirect::~CRedirect()
{
    TerminateChildProcess();
}

bool CRedirect::StartChildProcess(bool bShowChildWindow)
{
    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);

    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
#ifdef Q_OS_WIN32
    process->start("cmd.exe");
#else
    process->start("sh");
    process->write("pwd\n");
#endif
    return true;
}

bool CRedirect::IsChildRunning() const
{
    return true;
}

void CRedirect::TerminateChildProcess()
{

}

// Thread to monitoring the child process.

int CRedirect::ProcessThread()
{
    return 0;
}

// Function that write to the child stdin.

void CRedirect::WriteChildStdIn(QString szInput)
{
    process->write(szInput.toLatin1());
#ifdef Q_OS_UNIX
    process->write("pwd\n");
#endif
}

 void CRedirect::readyReadStandardOutput()
 {
     QString str = process->readAllStandardOutput();

     emit OnChildStdOutWrite(str);
 }

 void CRedirect::readyReadStandardError()
 {
     emit OnChildStdErrWrite(QString(process->readAllStandardError()));
 }
