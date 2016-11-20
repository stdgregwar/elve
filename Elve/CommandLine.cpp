#include "CommandLine.h"
#include <QJsonDocument>
#include <QFile>

//ALICE ==================================================
namespace alice {


template<>
struct store_info<SharedEGraph>
{
    static constexpr const char* key         = "graphs";  /* internal key, must be unique for each store */
    static constexpr const char* option      = "graph";   /* long flag for general commands, e.g., `store --aig` */
    static constexpr const char* mnemonic    = "g";     /* short flag for general commands, e.g., `store -a` */
    static constexpr const char* name        = "Graph";   /* singular name for option descriptions */
    static constexpr const char* name_plural = "Graphs";  /* plural name for option descriptions */
};

struct io_graph_tag_t {};

template<>
inline std::string store_entry_to_string<SharedEGraph>( SharedEGraph const& eg )
{
    const SharedGraph& g = eg->graph();
    return boost::str( boost::format( "%s i/o = %d/%d, nodecount = " ) % g->filename() % g->inputCount() % g->outputCount() % g->nodeCount());
}

template<>
inline void print_store_entry_statistics<SharedEGraph>( std::ostream& os, SharedEGraph const& eg)
{
    const SharedGraph& g = eg->graph();
    os << "nodecount : " << g->nodeCount()
       << ", inputs : " << g->inputCount()
       << ", outputs :" << g->outputCount()
       << "levels : " << g->highestLevel();
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
    EGraph::fromFile(QString::fromStdString(file));
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

}

//SINGLETON ==============================================
CommandLine* CommandLine::sInstance = nullptr;

CommandLine::CommandLine() : mCli("elve")
{

}

CommandLine& CommandLine::get()
{
    if(!sInstance) {
        sInstance = new CommandLine();
    }
    return *sInstance;
}



bool CommandLine::run_command(const QString& cmd)
{

}

QStringList CommandLine::completion(const QString& base)
{

}
