#include "CommandLine.h"
#include <QJsonDocument>
#include <QFile>
#include <sstream>

#include <alice/commands/show.hpp>
#include <interfaces/LayoutPlugin.h>

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
        MainWindow::get().newWindowWithFile(element,element->graph()->filename());
        return false; //Deprecated //TODO change this
    }

    command::log_opt_t log() const
    {
        return boost::none;
    }
};


//Layout command
class LayoutCommand : public command
{
public:
    LayoutCommand(LayoutPlugin* pl, const environment::ptr& env) : command(env, (pl->name() + " Layout").toStdString()),
      mLayout(pl)
    {
        pod.add("index",1);
        opts.add_options()
                ("index,i",po::value(&id),
                 ("id of the graph to set Layout " + pl->name() + " on.").toStdString().c_str());

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        auto& graphs = env->store<SharedEGraph>();
        return {
            {[this,&graphs] {return (!is_set("id") && !graphs.empty()) || (id<graphs.size() && id > -1);},
                "if set id must be in store range, store must not be empty"}
        };
    }

    bool execute() override {
        auto& graphs = env->store<SharedEGraph>();
        SharedEGraph eg;
        if(is_set("index")){
            eg = graphs[id];
        } else {
            eg = graphs.current();
        }
        eg->setLayout(mLayout->create());
        return true;
    }
private:
    unsigned id = 0;
    LayoutPlugin* mLayout;
};

//Layout command
class LoaderCommand : public command
{
public:
    LoaderCommand(GraphLoaderPlugin* pl, const environment::ptr& env) : command(env, (pl->formatName() + " Loader").toStdString()),
      mLoader(pl)
    {
        pod.add("filename",1);
        opts.add_options()
                ("filename,f",po::value(&mFilename),
                 ("filename of the " + pl->formatName() + " file to load").toStdString().c_str());

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        return  {
            file_exists_if_set( *this, process_filename( mFilename ),"filename")
        };
    }

    bool execute() override {
        auto& graphs = env->store<SharedEGraph>();
        graphs.extend();
        graphs.current() = std::make_shared<EGraph>(mLoader->load(QString::fromStdString(mFilename)));
        return true;
    }
private:
    std::string mFilename;
    GraphLoaderPlugin* mLoader;
};

}


//=================================================================================================================================

using namespace std;
using namespace alice;

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
}

alice::cli_store<SharedEGraph>& CommandLine::store() {
    return mCli.env->store<SharedEGraph>();
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
