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
    using pair_type = NodeDatas::value_type;
    for(const pair_type& p : data->nodeDatas()) {
        addNode(p.second);
    }
    for(const pair_type& p : data->nodeDatas()) {
        for(const NodeID& did : p.second.dependencies()) {
            addEdge(did,p.first);
        }
    }
}

Graph::Graph(const SharedData& data, const NodeIDSet &groups, const Aliases &aliases)
    : mData(data), mAliases(aliases)
{
    //Build groups data
    for(const NodeID& id : groups) {
        std::pair<NodeDatas::iterator,bool> p = mGroupsData.emplace(std::piecewise_construct,
                            std::forward_as_tuple(id),
                            std::forward_as_tuple(NodeData(id,{},CLUSTER)));
        Node* n = addNode(p.first->second);
        //Todo construct inner graph if needed
    }

    using pair_type = NodeDatas::value_type;

    for(const pair_type& p : data->nodeDatas()) {
        addNode(p.second);
    }
    for(const pair_type& p : data->nodeDatas()) {
        for(const NodeID& did : p.second.dependencies()) {
            addEdge(alias(did),alias(p.first));
        }
    }
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

NodeID Graph::uniqueID(const NodeID& base) const {
    NodeID current = base;
    unsigned suffix = 1;
    repeat:
    for(const auto& p : mNodes) {
        const NodeID& id = p.first;
        if(id == current) {
           current = base + "_" + std::to_string(suffix++);
           goto repeat;
        }
    }
    return current;
}


SharedGraph Graph::ungroup(const NodeID &cluster) {

//    auto it = mNodes.find(cluster);
//    if(it == mNodes.end()) {
//        return shared_from_this();
//    }
//    const Node& cn = it->second;
//    if(!cn.getClusteredGraph()) { //Node is not a cluster
//        return shared_from_this();
//    }

//    const SharedGraph& cg = cn.getClusteredGraph();

//    NodeDescriptions gnodes; gnodes.reserve(nodeCount()+cg->nodeCount());
//    AdjacencyList gadj; gadj.reserve(gnodes.size()*2); //True for AIG

//    using pair_type = NodesByID::value_type;
//    for(const pair_type& p : nodes()) {
//        const Node& n = p.second;
//        if(n.id() != cluster) {
//            gnodes.emplace(n.id(),n);
//            for(const Node* an : n.ancestors()) { //Add all ancestors that are not part of the cluster
//                if(an->id() != cluster) {
//                    gadj.push_back({an->id(),n.id()});
//                }
//            }
//        }
//    }

//    for(const pair_type& p : cg->nodes()) {
//        const Node& n = p.second;
//        if(n.isOutput()) {
//            for(const Node* an : n.ancestors()) {//Add
//                gadj.push_back({an->id() ,alias(n.id())});
//            }
//        } else if(!n.isInput()) {
//            gnodes.emplace(n.id(),n);
//            for(const Node* an : n.ancestors()) { //Add all ancestors that are not part of the cluster
//                if(an->isInput()) { //Make sure node is connected to right node/cluster
//                    gadj.push_back({alias(an->id()),n.id()});
//                } else {
//                    gadj.push_back({an->id(),n.id()});
//                }
//            }
//        }
//    }

//    return make_shared<Graph>(gnodes,gadj,aliasesWithout(cluster));
    return SharedGraph();
}

SharedGraph Graph::group(const NodeIDSet &toGroup, const NodeID &groupID) {
    if(toGroup.size() < 2) {
        return shared_from_this();
    }

    NodeID trueID = uniqueID(groupID);

    NodeIDSet groups;// groups.reserve(mGroupsData.size()+1);
    Aliases aliases = mAliases;
    aliases.reserve(aliases.size()+toGroup.size());

    for(const NodeID& id : toGroup) {
        aliases.emplace(id,trueID);
    }

    using pair_type = NodeDatas::value_type;
    for(const pair_type& p : mGroupsData) {
        groups.insert(p.first);
    }
    groups.insert(trueID);
    return make_shared<Graph>(mData,groups,aliases);
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
