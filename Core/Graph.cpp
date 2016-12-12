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

Graph::Graph(const SharedData& data, const NodeDatas &groups, const Aliases &aliases, const NodeIDSet& excluded)
    : mData(data), mGroupsData(groups), mAliases(aliases), mExcluded(excluded)
{
    //Build groups data
    /*NodeID i = mData->nodeDatas().size();
    for(const NodeName& name : groups) {
        mGroupsData.emplace_back(NodeData(i,name,{},CLUSTER));
        //Todo construct inner graph if needed
    }*/

    using pair_type = NodeDatas::value_type;

    for(const NodeData& d : data->nodeDatas()) {
        if(!excluded.count(d.id())) {
            addNode(d);
        }
    }
    for(const NodeData& d : data->nodeDatas()) {
        for(const NodeID& did : d.dependencies()) {
            addEdge(alias(did),alias(d.id()));
        }
    }
}

NodeID Graph::newID() const {
    NodeID i = mData->nodeDatas().size()+mGroupsData.size();
}

const QString& Graph::filename() const {
    return mData->mFilename;
}

const NodesByID& Graph::nodes() const
{
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
    if(d.type() == NodeType::OUTPUT) {
        mOutputs.push_back(n);
    } else if (d.type() == NodeType::INPUT) {
        mInputs.push_back(n);
    }
    return n;
}

void Graph::addEdge(const NodeID& from, const NodeID& to)
{
    if(from != to) {
        mNodes.at(from).addChild(&mNodes.at(to));
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

    /*NodeIDSet groups;
    using pair_type = NodeDatas::value_type;
    for(const NodeData& d : mGroupsData) {
        if(d.id() != cluster) {
            groups.insert(d.id());
        }
    }
    return make_shared<Graph>(mData,groups,aliasesWithout(cluster));*/
}

SharedGraph Graph::group(const NodeIDSet &toGroup, const NodeName &groupName) {
    /*if(toGroup.size() < 2) {
        return shared_from_this();
    }

    NodeName trueName = uniqueName(groupID);
    NodeID i = newID();

    NodeIDSet groups;// groups.reserve(mGroupsData.size()+1);
    Aliases aliases = mAliases;
    aliases.reserve(aliases.size()+toGroup.size());

    for(const NodeID& id : toGroup) {
        aliases.emplace(id,i);
    }

    using pair_type = NodeDatas::value_type;
    for(const pair_type& p : mGroupsData) {
        groups.insert(p.first);
    }
    groups.insert(trueID);
    return make_shared<Graph>(mData,groups,aliases);*/
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
    return (*std::max_element(mInputs.begin(),mInputs.end(),
        [](const Node* a,const Node* b) {
            return a->IOIndex() < b->IOIndex();
    }))->IOIndex();
}

size_t Graph::maxOutputIndex() const
{
    return (*std::max_element(mOutputs.begin(),mOutputs.end(),
        [](const Node* a,const Node* b) {
            return a->IOIndex() < b->IOIndex();
    }))->IOIndex();
}

QJsonObject Graph::json() const
{
    QJsonObject main;
    main.insert("graph_data",mData->json());

    //Todo insert clustering details here

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
    return make_shared<Graph>(sdata);
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
