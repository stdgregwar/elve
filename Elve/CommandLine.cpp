#include "CommandLine.h"
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
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


class PluginCommand : public command
{
public : PluginCommand(Plugin* pl,const environment::ptr& env,const std::string& name) : command(env,name)
    {
        pl->set_vmap(&vm);
    }
};

//Layout command
class LayoutCommand : public PluginCommand
{
public:
    LayoutCommand(LayoutPlugin* pl, const environment::ptr& env) : PluginCommand(pl,env, (pl->name() + " Layout").toStdString()),
      mLayout(pl)
    {
        pod.add("index",1);
        opts.add_options()
                ("index",po::value(&id),
                 ("id of the graph to set Layout " + pl->name() + " on.").toStdString().c_str());

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        Store& graphs = env->store<SharedEGraph>();
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

//Loader command
class LoaderCommand : public PluginCommand
{
public:
    LoaderCommand(GraphLoaderPlugin* pl, const environment::ptr& env) : PluginCommand(pl,env, (pl->formatName() + " Loader").toStdString()),
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
        graphs.push(std::make_shared<EGraph>(mLoader->load(QString::fromStdString(mFilename))));
        return true;
    }
private:
    std::string mFilename;
    GraphLoaderPlugin* mLoader;
};

//Export command
class ExportCommand : public PluginCommand
{
public:
    ExportCommand(FileExporterPlugin* pl, const environment::ptr& env) : PluginCommand(pl,env, (pl->formatName() + " Saver").toStdString()),
      mSaver(pl)
    {
        pod.add("filename",1);
        opts.add_options()
                ("filename,f",po::value(&mFilename),
                 ("filename of the " + pl->formatName() + " file to save").toStdString().c_str());

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        return  {
            //file_exists_if_set( *this, process_filename( mFilename ),"filename")
        };
    }

    bool execute() override {
        auto& graph = env->store<SharedEGraph>().current();
        //Todo make sure scene is available, or find better way
        mSaver->exportGraph(QString::fromStdString(mFilename),graph);
        return true;
    }

private:
    std::string mFilename;
    FileExporterPlugin* mSaver;
};

//Transform command
class TransformCommand : public PluginCommand
{
public:
    TransformCommand(GraphTransformPlugin* pl, const environment::ptr& env) : PluginCommand(pl,env, (pl->name()).toStdString()),
      mTrans(pl)
    {
        opts.add_options()
                ("input,i",po::value(&mInput),
                 "index of the graph to transform (default : current)")
                ("output,o",po::value(&mOutput),"output index for the graph (opt)");

        opts.add(pl->opts()); //Add plugin options themselves
    }

    rules_t validity_rules() const override {
        return  {
            //file_exists_if_set( *this, process_filename( mFilename ),"filename")
        };
    }

    bool execute() override {
        SharedEGraph input;
        if(is_set("input")) {
            input = env->store<SharedEGraph>()[mInput];
        } else {
            input = env->store<SharedEGraph>().current();
        }
        SharedEGraph output = mTrans->transform(input);
        if(is_set("output")) {
            env->store<SharedEGraph>()[mOutput] = output;
        } else {
            env->store<SharedEGraph>().current() = output;
        }
        env->store<SharedEGraph>().notify();
        if(output->view() && mTrans->type() == SELECTION) {
            output->view()->updateSelectionColor();
        }
    }
private:
    GraphTransformPlugin* mTrans;
    int mInput;
    int mOutput;
};

class SelectCmd : public command
{
public:
    SelectCmd(const environment::ptr& env) : command(env,"Select") {
        pod.add("mask",1).add("nodeIDs",-1);

        opts.add_options()
                ("mask,m",po::value(&mask)->default_value(0),"Selection mask index")
                ("index,i",po::value(&sid))
                ("clear,c","clear existing selection")
                ("all,a","select all")
                ("add","add nodes")
                ("sub","sub nodes")
                ("nodeIDs",po::value(&ids),"node ids to process");
    }
    bool execute() override {
        SharedEGraph eg = env->store<SharedEGraph>().current();
        Selection& s = eg->selection(mask);
        if(is_set("clear")) {
            s.clear();
        }
        if(is_set("all")) {
            for(const auto& p : eg->graph()->nodes()) {
                s.add(p.first);
            }
        }
        auto op = static_cast<void(Selection::*)(const NodeID&)>(&Selection::add);
        if(is_set("sub")) {
            op = static_cast<void(Selection::*)(const NodeID&)>(&Selection::sub);
        }
        for(const NodeID& id : ids) {
            (s.*op)(id);
        }
        if(eg->view()) {
            eg->view()->updateSelectionColor();
        }
    }
private:
    int sid;
    NodeIDs ids;
    int mask;
};

class GroupCmd : public command
{
public:
    GroupCmd(const environment::ptr& env) : command(env,"Group"),name("group") {
        pod.add("mask",1).add("name",1);

        opts.add_options()
                ("mask,m",po::value(&mask)->default_value(0),"Selection mask index")
                ("name,n",po::value(&name)->default_value(name),"Name of the group")
                ("index,i",po::value(&sid));
    }

    bool execute() override {
        SharedEGraph eg = env->store<SharedEGraph>().current();
        SharedEGraph grouped =  eg->group(eg->selection(mask),name);
        env->store<SharedEGraph>().current() = grouped;
        if(eg->view()) eg->view()->setGraph(grouped);
    }
private:
    NodeName name;
    int sid;
    int mask;
};

class UngroupCmd : public command
{
public:
    UngroupCmd(const environment::ptr& env) : command(env,"Ungroup") {
        pod.add("mask",1).add("name",1);

        opts.add_options()
                ("mask,m",po::value(&mask)->default_value(0),"Selection mask index")
                ("index,i",po::value(&sid));
    }

    bool execute() override {
        SharedEGraph eg = env->store<SharedEGraph>().current();
        Selection& s = eg->selection(mask);
        SharedEGraph ungrouped = eg->ungroup(s);
        env->store<SharedEGraph>().current() = ungrouped;
        if(eg->view()) eg->view()->setGraph(ungrouped);
    }
private:
    int sid;
    int mask;
};

class clustercmd : public command
{
public:
    clustercmd(const environment::ptr& env) : command(env,"Ungroup") {
    }

    bool execute() override {
        SharedEGraph eg = env->store<SharedEGraph>().current();
        SharedGraph ng = eg->graph()->clusterize(1);
        SharedEGraph neg = std::make_shared<EGraph>(ng,eg->positions());
        neg->setLayout(eg->layout());
        neg->setView(eg->view());
        env->store<SharedEGraph>().current() = neg;
        if(neg->view()) neg->view()->setGraph(neg);
    }
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
    mCli.set_category("Selection");
    mCli.insert_command("select",make_shared<SelectCmd>(mCli.env));
    mCli.insert_command("group",make_shared<GroupCmd>(mCli.env));
    mCli.insert_command("ungroup",make_shared<UngroupCmd>(mCli.env));
    mCli.insert_command("cluster",make_shared<clustercmd>(mCli.env));
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
