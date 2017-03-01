#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>

#include "alice/allalice.h"
#include "Singleton.h"
#include <interfaces/GraphWidgetListener.h>

#include <EGraph.h>
#include <QString>


namespace Elve {

typedef alice::cli_store<SharedEGraph> Store;

class CommandLine : public QObject, public Singleton<CommandLine>, public GraphWidgetListener
{
    friend class Singleton<CommandLine>;
    Q_OBJECT
public:
    bool run_command(const QString& cmd, std::ostream& out, std::ostream &cerr);
    QStringList completion(const QString& base);
    void graphChanged(SharedEGraph oldGraph, SharedEGraph newGraph) override;
    Store& store();
private:
    CommandLine(); //Private constructor for singleton
    void init();
    void setupPluginsCommands();
    alice::cli_main<SharedEGraph> mCli;
};

}
#endif // COMMANDLINE_H
