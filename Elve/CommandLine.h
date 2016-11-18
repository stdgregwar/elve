#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include "alice/alice.hpp"
#include <Graph.h>
#include <QString>

class CommandLine : public QObject
{
    Q_OBJECT
public:
    CommandLine& get();
    bool run_command(const QString& cmd);
    QStringList completion(const QString& base);
private:
    CommandLine(); //Private constructor for singleton
    static CommandLine* sInstance;

    alice::cli_main<SharedGraph> mCli;
};

#endif // COMMANDLINE_H
