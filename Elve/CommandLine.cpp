#include "CommandLine.h"
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <sstream>

#include <alice/commands/show.hpp>
#include <interfaces/LayoutPlugin.h>
#include <chrono>

#include "commands/Chrono.h"
#include "commands/ClearAll.h"
#include "commands/Cluster.h"
#include "commands/Export.h"
#include "commands/Group.h"
#include "commands/Layout.h"
#include "commands/Loader.h"
#include "commands/Select.h"
#include "commands/Transform.h"
#include "commands/Ungroup.h"
#include "commands/Look.h"

#include "MainWindow.h"

//ALICE ==================================================
namespace alice {

using namespace Elve;

template<>
struct store_info<SharedEGraph>
{
    static constexpr const char* key         = "graph";  /* internal key, must be unique for each store */
    static constexpr const char* option      = "gr";   /* long flag for general commands, e.g., `store --aig` */
    static constexpr const char* mnemonic    = "g";     /* short flag for general commands, e.g., `store -a` */
    static constexpr const char* name        = "Graph";   /* singular name for option descriptions */
    static constexpr const char* name_plural = "Graphs";  /* plural name for option descriptions */
};

struct io_graph_tag_t {};

template<>
inline std::string store_entry_to_string<SharedEGraph>( SharedEGraph const& eg )
{
    const SharedGraph& g = eg->graph();
    qDebug() << "node count" << g->nodeCount();
    return boost::str( boost::format( "%s i/o = %d/%d, nodecount = %d" ) % g->filename().toStdString() % g->inputCount() % g->outputCount() % g->nodeCount());
}

template<>
inline void print_store_entry_statistics<SharedEGraph>( std::ostream& os, SharedEGraph const& eg)
{
    const SharedGraph& g = eg->graph();
    os << "nodecount : " << g->nodeCount()
       << ", inputs : " << g->inputCount()
       << ", outputs : " << g->outputCount()
       << ", levels : " << g->highestLevel() << std::endl;
}

template<>
inline bool store_can_read_io_type<SharedEGraph, io_graph_tag_t>( command& cmd )
{
    return true;
}

/* implement `read graph`*/
template<>
inline SharedEGraph store_read_io_type<SharedEGraph, io_graph_tag_t>( const std::string& file, const command& cmd )
{
    return ExtendedGraph::fromFile(QString::fromStdString(file));
}

/* enable `write Graphs`*/
template<>
inline bool store_can_write_io_type<SharedEGraph, io_graph_tag_t>( command& cmd )
{
    return true;
}

/* implement `write Graphs */
template<>
inline void store_write_io_type<SharedEGraph, io_graph_tag_t>( SharedEGraph const& eg, const std::string& filename, const command& cmd )
{
    eg->toFile(QString::fromStdString(filename));
}

//Implement show for Graphs
template<>
struct show_store_entry<SharedEGraph>
{
    show_store_entry<SharedEGraph>(command& cmd){}

    bool operator()(SharedEGraph& element, const std::string& dotname, const command& cmd, std::ostream& out )
    {
        MainWindow::get().newWindowWithFile(element,element->graph()->filename());
        return false; //Deprecated //TODO change this
    }

    command::log_opt_t log() const
    {
        return boost::none;
    }
};

}

//=================================================================================================================================


namespace Elve {

using namespace std;
using namespace alice;

CommandLine::CommandLine() : mCli("elve")
{
    init();
}

void CommandLine::init()
{
    using namespace alice;
    using namespace Elve;
    auto& cli = mCli;
    mCli.set_category("I/0");
    ADD_READ_COMMAND(graph,"Graph");
    ADD_WRITE_COMMAND(graph,"Graph");
    mCli.init(0,{},std::cout);
    mCli.set_category("Selection");
    mCli.insert_command("select",make_shared<SelectCommand>(mCli.env));
    mCli.insert_command("group",make_shared<GroupCommand>(mCli.env));
    mCli.insert_command("ungroup",make_shared<UngroupCommand>(mCli.env));
    mCli.insert_command("cluster",make_shared<ClusterCommand>(mCli.env));
    mCli.set_category("utils");
    mCli.insert_command("chrono",make_shared<ChronoCommand>(mCli.env));
    mCli.insert_command("clearall",make_shared<ClearAllCommand>(mCli.env));
    setupPluginsCommands();
}

void CommandLine::setupPluginsCommands() {
    mCli.set_category("Layouts");
    for(LayoutPlugin* pl : PluginManager::get().layouts()) {
        mCli.insert_command(pl->cliName()+"_layout", make_shared<LayoutCommand>(pl,mCli.env));
    }

    mCli.set_category("Loaders");
    for(GraphLoaderPlugin* pl : PluginManager::get().loaders()) {
        mCli.insert_command("load_" + pl->cliName(), make_shared<LoaderCommand>(pl,mCli.env));
    }

    mCli.set_category("Savers/Exporters");
    for(FileExporterPlugin* pl : PluginManager::get().exporters()) {
        mCli.insert_command("save_" + pl->cliName(), make_shared<ExportCommand>(pl,mCli.env));
    }

    mCli.set_category("Transforms");
    for(GraphTransformPlugin* pl : PluginManager::get().transforms()) {
        mCli.insert_command(pl->cliName(), make_shared<TransformCommand>(pl,mCli.env));
    }

    mCli.set_category("Looks");
    for(LookFactoryPlugin* pl : PluginManager::get().looks()) {
        mCli.insert_command(pl->cliName()+"_look", make_shared<LookCommand>(pl,mCli.env));
    }
}

Store &CommandLine::store() {
    return mCli.env->store<SharedEGraph>();
}

void CommandLine::graphChanged(SharedEGraph oldGraph, SharedEGraph newGraph) {
    Store& st = mCli.env->store<SharedEGraph>();

    qDebug() << "Graph modified";
    for(int i = 0; i < st.data().size(); i ++) {
        if(st.data()[i] == oldGraph) {
            st.set_current_index(i);
            st.current() = newGraph;
            qDebug() << "previous found";
            break;
        }
    }
    st.notify(Store::ALL);
}

bool CommandLine::run_command(const QString& cmd, std::ostream& out,std::ostream& cerr)
{
    //try {
        mCli.run_line(cmd.toStdString(),out,cerr);
        store().current()->history().add(cmd.toStdString());
    //} catch(const std::exception& e) {
    //    cerr << e.what() << std::endl;
    //}
}

QStringList CommandLine::completion(const QString& base)
{
    return QStringList();
}

}
