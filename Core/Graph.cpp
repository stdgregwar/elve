#include "Graph.h"
#include <string>
#include "Node.h"

#include <algorithm>
#include <QDebug>
#include <QJsonArray>
#include "utils.h"

using namespace std;

Graph::Graph(const SharedData &data) : mData(data)
{
    for(const NodeData& d : data->nodeDatas()) {
        addNode(d);
    }
    for(const NodeData& d : data->nodeDatas()) {
        for(const Dependency& dep : d.dependencies()) {
            addEdge(dep.id,dep.from,d.id(),dep.to);
        }
    }
}

Graph::Graph(const SharedData& data, const SparseData &extraData, const Aliases &aliases, const NodeIDSet& excluded)
    : mData(data), mExtraData(extraData), mAliases(aliases), mExcluded(excluded)
{
    //Build groups data
    using pair_type = SparseData::value_type;
    for(const pair_type& p : mExtraData) {
        if(!mExcluded.count(p.first)) {
            addNode(p.second);
        } else {
            qDebug() << "Extra data" << p.second.name().c_str() << "was exluded";
        }
    }

    for(const NodeData& d : data->nodeDatas()) {
        if(!mExcluded.count(d.id())) {
            addNode(d);
        }
    }

    for(const NodeData& d : data->nodeDatas()) {
        NodeID tid = alias(d.id());
        if(!mExcluded.count(tid)) {
            for(const Dependency& dep : d.dependencies()) {
                NodeID tpid = alias(dep.id);
                if(!mExcluded.count(tpid)) {
                    addEdge(tpid,dep.from,tid,dep.to);
                }
            }
        }
    }
}

NodeID Graph::newID() const {
    return ++mLastId;
    /*NodeID i = mData->nodeDatas().size()+mExtraData.size();
    NodesByID::const_iterator it;
    while((it = mNodes.find(i)) != mNodes.end()) {
        i++;
    }
    return i;*/
}

const QString& Graph::filename() const {
    return mData->mFilename;
}

const NodesByID& Graph::nodes() const {
    return mNodes;
}

size_t Graph::nodeCount() const {
    return mNodes.size();
}

const NodeID& Graph::alias(const NodeID& id) const {
    auto it = mAliases.find(id);
    if(it != mAliases.end()) return alias(it->second);
    return id;
}

Node* Graph::addNode(const NodeData& d) {
    auto pi = mNodes.emplace(d.id(),d);

    Node* n = &pi.first->second;
    if(d.type() == NodeType::OUTPUT or d.type() == NodeType::OUTPUT_CLUSTER) {
        mOutputs.push_back(n);
    } else if (d.type() == NodeType::INPUT or d.type() == NodeType::INPUT_CLUSTER) {
        mInputs.push_back(n);
    }
    if(n->id() > mLastId) mLastId = n->id();
    return n;
}

void Graph::addEdge(const NodeID& from, Index fi, const NodeID& to, Index ti) {
    if(from != to) {
        mNodes.at(from).addChild(&mNodes.at(to),fi,ti);
    }
}

const NodeData& Graph::data(const NodeID& id) const {
    if(id < mData->nodeDatas().size()) {
        return mData->nodeDatas().at(id);
    } else {
        return mExtraData.at(id);
    }
}

///CLUSTERING SECTION

void _descend(const Node *n, vector<NodeIDSet> &sets, NodeIDSet &mainSet);

void _collectClusterables(const Node* n, NodeIDSet& set, vector<NodeIDSet>& sets, NodeIDSet& mainSet) {
    set.insert(n->id());
    mainSet.insert(n->id());
    vector<const Node*> collected; collected.reserve(n->ancestors().size());
    for(const Node* cn : n->ancestors()) {
        if(std::all_of(cn->children().begin(),
                       cn->children().end(),
                       [&set](const Node* an){return set.count(an->id());})
                and !cn->isInput() and !cn->isOutput() and !mainSet.count(cn->id())) {
            set.insert(cn->id()); //insert if all ancestors are in accumulator
            collected.push_back(cn);
            mainSet.insert(cn->id());
        } else {
            _descend(cn,sets,mainSet);
        }
    }
    for(const Node* cn : collected) { //doing breadth first visit
         _collectClusterables(cn,set,sets,mainSet);
    }
}

void _descend(const Node* n, vector<NodeIDSet>& sets, NodeIDSet& mainSet) {
    if(!mainSet.count(n->id())) {
        NodeIDSet collect; collect.reserve(20);
        _collectClusterables(n,collect,sets,mainSet);
        sets.push_back(collect);
    }
}

SharedGraph Graph::clusterize(size_t level) {
    if(level == 0) {
        return shared_from_this();
    }
    vector<NodeIDSet> sets;
    NodeIDSet mainSet; mainSet.reserve(nodeCount());
    for(Node* out : outputs()) {
        //qDebug() << "output" << out->name().c_str();
        NodeIDSet collect;
        _collectClusterables(out,collect,sets,mainSet);
        collect.erase(out->id());
        sets.push_back(collect);
    }
    SharedGraph g = shared_from_this();
    int i = 0;
    g = g->fastGroup(sets,"cluster");
    qDebug() << "shrinked a" << nodeCount() << "nodes graph to a" << g->nodeCount() << "one";
    return g->clusterize(level-1); //Does not do anything...
}

///END OF CLUSTERING SECTION

NodeName Graph::uniqueName(const NodeName& base) const {
    NodeName current = base;
    unsigned suffix = 1;
    repeat:
    for(const auto& p : mNodes) {
        const NodeName& id = p.second.name();
        if(id == current) {
           current = base + "_" + std::to_string(suffix++);
           goto repeat;
        }
    }
    return current;
}


SharedGraph Graph::ungroup(const NodeID &cluster) {
    if(!mExtraData.count(cluster)) {
        return shared_from_this();
    }
    SparseData extras(mExtraData);
    NodeIDSet excl = excludedWithout(extras.at(cluster).dependencies());
    extras.erase(cluster);
    return make_shared<Graph>(mData,extras,aliasesWithout(cluster),excl);
}

SharedGraph Graph::fastGroup(const vector<NodeIDSet>& groups, const NodeName& basename) {
    //Gather stats
    size_t contentSize = 0;
    for(const NodeIDSet& group : groups) {
        contentSize += group.size();
    }

    SparseData extra; extra.reserve(mExtraData.size()+groups.size());

    using pair_type = SparseData::value_type;
    for(const pair_type& p : mExtraData) {
        extra.emplace(p.first,p.second);
    }

    Aliases aliases(mAliases);
    NodeIDSet excluded(mExcluded); excluded.reserve(mExcluded.size()+contentSize);
    aliases.reserve(aliases.size()+contentSize);



    for(const NodeIDSet& group : groups) {
        NodeID i = newID();
        if(group.size() < 2) { //Ignore groups of one or less elements
            continue;
        }
        Dependencies deps; deps.reserve(group.size());
        deps.insert(deps.end(),group.begin(),group.end());

        float av_index = 0;
        for(const NodeID& id : group) {
            aliases.emplace(id,i);
            excluded.insert(id);
            av_index += data(id).ioIndex();
        }
        av_index /= group.size();

        extra.emplace(i,NodeData(i,basename + to_string(i),deps,CLUSTER,{},av_index));
        //i++;
    }
    return make_shared<Graph>(mData,extra,aliases,excluded);
}

SharedGraph Graph::group(const NodeIDSet &toGroup, const NodeID& i,const NodeName &groupName) {
    if(toGroup.size() < 2) {
        return shared_from_this();
    }

    const NodeName& trueName = groupName;//uniqueName(groupName);

    SparseData extra; extra.reserve(mExtraData.size()+1);
    Aliases aliases(mAliases);
    NodeIDSet excluded(mExcluded); excluded.reserve(mExcluded.size()+toGroup.size());
    aliases.reserve(aliases.size()+toGroup.size());
    Dependencies deps; deps.reserve(toGroup.size());
    deps.insert(deps.end(),toGroup.begin(),toGroup.end());

    float av_index = 0;
    for(const NodeID& id : toGroup) {
        aliases.emplace(id,i);
        excluded.insert(id);
        av_index += data(id).ioIndex();
    }
    av_index /= toGroup.size();

    using pair_type = SparseData::value_type;
    for(const pair_type& p : mExtraData) {
        extra.emplace(p.first,p.second);
    }

    //Handle cluster types
    const NodeData& first = data(*toGroup.begin());
    NodeType t = CLUSTER;
    switch (first.type()) {
    case INPUT:
    case INPUT_CLUSTER:
        t = INPUT_CLUSTER;
        break;
    case OUTPUT:
    case OUTPUT_CLUSTER:
        t = OUTPUT_CLUSTER;
        break;
    default:
        break;
    }

    extra.emplace(i,NodeData(i,trueName,deps,t,{},av_index));
    return make_shared<Graph>(mData,extra,aliases,excluded);
}

const NodePtrs& Graph::inputs() {
    return mInputs;
}

const NodePtrs& Graph::outputs() {
    return mOutputs;
}

size_t Graph::inputCount() const
{
    return mInputs.size();
}

size_t Graph::outputCount() const
{
    return mOutputs.size();
}

size_t Graph::maxInputIndex() const
{
    size_t i = 0;
    for(const Node* in : mInputs) {
        if(in->IOIndex() > i) {
            i = in->IOIndex();
        }
    }
    return i == 0 ? 1 : i;
}

size_t Graph::maxOutputIndex() const
{
    size_t i = 0;
    for(const Node* in : mOutputs) {
        if(in->IOIndex() > i) {
            i = in->IOIndex();
        }
    }
    return i == 0 ? 1 : i;
}

QJsonObject Graph::json() const
{
    QJsonObject main;
    main.insert("graph_data",mData->json());

    //Todo insert clustering details here

    {
        QJsonArray extra;
        using pair_type = SparseData::value_type;
        for(const pair_type& p : mExtraData) {
            extra.append(p.second.json());
        }
        main.insert("extra_data",extra);
    }
    {
        QJsonArray als;
        using pair_type = Aliases::value_type;
        for(const pair_type& p : mAliases) {
            als.append(QJsonArray{(int)p.first,(int)p.second});
        }
        main.insert("aliases",als);
    }
    QJsonArray excl;
    for(const NodeID& id : mExcluded) {
        excl.append((int)id);
    }
    main.insert("excluded",excl);
    return main;
}

NodeLevel Graph::highestLevel() const {
    using pair_type = NodesByID::value_type;

    return max_element(mNodes.begin(),mNodes.end(),
        [](const pair_type& a, const pair_type& b)->bool {
            return a.second.level() < b.second.level();
        })->second.level();
}

SharedGraph Graph::fromJson(const QJsonObject& obj)
{
    SharedData sdata = make_shared<GraphData>(obj.value("graph_data").toObject());
    //Todo find clustering primitives here
    SparseData extra;
    QJsonArray jextr = obj.value("extra_data").toArray();
    extra.reserve(jextr.size());
    for(const QJsonValue& v : jextr) {
        QJsonObject obj = v.toObject();
        extra.emplace(obj.value("id").toInt(),obj);
    }
    Aliases als;
    QJsonArray jals = obj.value("aliases").toArray();
    als.reserve(jals.size());
    for(const QJsonValue& v : jals) {
        QJsonArray pair = v.toArray();
        als.emplace(pair.at(0).toInt(-1),pair.at(1).toInt(-1));
    }
    NodeIDSet excl;
    QJsonArray jexcl = obj.value("excluded").toArray();
    excl.reserve(jexcl.size());
    for(const QJsonValue& v : jexcl) {
        excl.insert(v.toInt());
    }
    return make_shared<Graph>(sdata,extra,als,excl);
}

Aliases Graph::aliasesWithout(const NodeID& repl) const {
    Aliases als;
    for(const auto& p : mAliases) {
        if(p.second != repl) {
            als.insert(p);
        }
    }
    return als;
}

NodeIDSet Graph::excludedWithout(const Dependencies& ids) const {
    NodeIDSet excl(mExcluded);
    for(const Dependency& dep : ids) {
        excl.erase(dep.id);
    }
    return excl;
}
