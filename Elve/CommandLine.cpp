#include "CommandLine.h"
#include <QJsonDocument>
#include <QFile>
#include <sstream>

#include "alice/commands/show.hpp"

#include "MainWindow.h"

//ALICE ==================================================
namespace alice {


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
    return boost::str( boost::format( "%s i/o = %d/%d, nodecount = %d" ) % g->filename() % g->inputCount() % g->outputCount() % g->nodeCount());
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
    return EGraph::fromFile(QString::fromStdString(file));
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
        MainWindow::get().newWindowWithFile(element,QString::fromStdString(element->graph()->filename()));
        return false; //Deprecated //TODO change this
    }

    command::log_opt_t log() const
    {
        return boost::none;
    }
};

}

//Show command


//=================================================================================================================================

CommandLine::CommandLine() : mCli("elve")
{
    init();
}

void CommandLine::init()
{
    using namespace alice;
    auto& cli = mCli;
    mCli.set_category("I/0");
    ADD_READ_COMMAND(graph,"Graph");
    ADD_WRITE_COMMAND(graph,"Graph");
    mCli.init(0,{},std::cout);
}

bool CommandLine::run_command(const QString& cmd, std::ostream& out,std::ostream& cerr)
{
    try {
        mCli.run_line(cmd.toStdString(),out,cerr);
    } catch(const std::exception& e) {
        cerr << e.what() << std::endl;
    }
}

QStringList CommandLine::completion(const QString& base)
{
    return QStringList();
}
