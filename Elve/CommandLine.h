#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>

#include "alice/allalice.h"
#include "Singleton.h"

#include <EGraph.h>
#include <QString>

class CommandLine : public QObject, public Singleton<CommandLine>
{
    friend class Singleton<CommandLine>;
    Q_OBJECT
public:
    bool run_command(const QString& cmd, std::ostream& out, std::ostream &cerr);
    QStringList completion(const QString& base);
private:
    CommandLine(); //Private constructor for singleton
    void init();
    void setupPluginsCommands();
    alice::cli_main<SharedEGraph> mCli;
};

#endif // COMMANDLINE_H
