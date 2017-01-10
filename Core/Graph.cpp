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
        for(const NodeID& did : d.dependencies()) {
            addEdge(did,d.id());
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
            for(const NodeID& did : d.dependencies()) {
                NodeID tpid = alias(did);
                if(!mExcluded.count(tpid)) {
                    addEdge(tpid,tid);
                }
            }
        }
    }
}

NodeID Graph::newID() const {
    NodeID i = mData->nodeDatas().size()+mExtraData.size();
    return i;
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
    return n;
}

void Graph::addEdge(const NodeID& from, const NodeID& to) {
    if(from != to) {
        mNodes.at(from).addChild(&mNodes.at(to));
    }
}

const NodeData& Graph::data(const NodeID& id) const {
    if(id < mData->nodeDatas().size()) {
        return mData->nodeDatas().at(id);
    } else {
        return mExtraData.at(id);
    }
}

SharedGraph Graph::clusterize(size_t maxLevel) const {
    return nullptr;
}

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

SharedGraph Graph::group(const NodeIDSet &toGroup, const NodeID& i,const NodeName &groupName) {
    if(toGroup.size() < 2) {
        return shared_from_this();
    }

    NodeName trueName = uniqueName(groupName);

    SparseData extra; extra.reserve(mExtraData.size()+1);
    Aliases aliases(mAliases);
    NodeIDSet excluded(mExcluded); excluded.reserve(mExcluded.size()+toGroup.size());
    aliases.reserve(aliases.size()+toGroup.size());
    NodeIDs deps; deps.reserve(toGroup.size());
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

NodeIDSet Graph::excludedWithout(const NodeIDs& ids) const {
    NodeIDSet excl(mExcluded);
    for(const NodeID& id : ids) {
        excl.erase(id);
    }
    return excl;
}
