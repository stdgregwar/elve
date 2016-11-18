#include "CommandLine.h"

//ALICE ==================================================
namespace alice {


template<>
struct store_info<SharedGraph>
{
    static constexpr const char* key         = "graphs";  /* internal key, must be unique for each store */
    static constexpr const char* option      = "graph";   /* long flag for general commands, e.g., `store --aig` */
    static constexpr const char* mnemonic    = "g";     /* short flag for general commands, e.g., `store -a` */
    static constexpr const char* name        = "Graph";   /* singular name for option descriptions */
    static constexpr const char* name_plural = "Graphs";  /* plural name for option descriptions */
};

struct io_graph_tag_t {};

template<>
inline std::string store_entry_to_string<SharedGraph>( SharedGraph const& g )
{
  return boost::str( boost::format( "%s i/o = %d/%d, nodecount = " ) % g->filename() % g->inputCount() % g->outputCount() % g->nodeCount());
}

template<>
inline void print_store_entry_statistics<SharedGraph>( std::ostream& os, SharedGraph const& g)
{
    os << "nodecount : " << g->nodeCount()
       << ", inputs : " << g->inputCount()
       << ", outputs :" << g->outputCount()
       << "levels : " << g->highestLevel();
}

template<>
inline bool store_can_read_io_type<SharedGraph, io_graph_tag_t>( command& cmd )
{
  return true;
}

/* implement `read graph`*/
template<>
inline SharedGraph store_read_io_type<SharedGraph, io_graph_tag_t>( const std::string& filename, const command& cmd )
{
  return abc::Gia_AigerRead( (char*)filename.c_str(), 0, 0, 0 );
}

/* enable `write Graphs`*/
template<>
inline bool store_can_write_io_type<SharedGraph, io_graph_tag_t>( command& cmd )
{
  return true;
}

/* implement `write Graphs */
template<>
inline void store_write_io_type<SharedGraph, io_graph_tag_t>( SharedGraph const& aig, const std::string& filename, const command& cmd )
{
  abc::Gia_AigerWrite( aig, (char*)filename.c_str(), 1, 0 );
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
