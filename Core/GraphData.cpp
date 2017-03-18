#include "GraphData.h"
#include <QJsonObject>
#include <QJsonArray>

#include "Node.h"

namespace Elve {

GraphData::GraphData(const NodeDatas& nodesData,QString filename)
    : mDatas(nodesData), mFilename(filename)
{

}

GraphData::GraphData(const QJsonObject& obj) {
    mFilename = obj.value("filename").toString("unnamed");
    size_t size = obj.value("node_count").toInt(0);
    mDatas.reserve(size);
    QJsonArray nodes = obj.value("nodes").toArray();
    for(const QJsonValue& j : nodes) {
        mDatas.emplace_back(
                    //(j.toObject().value("id").toInt(0)),
                    (j.toObject())
                    );
    }
}

GraphData::Builder::Builder() : mID(0)
{

}

NodeID GraphData::Builder::id(const NodeName& name) {
    auto it = mIDs.find(name);
    if(it == mIDs.end()) {
        NodeID i = mID++;
        mNames.emplace(i,name);
        mIDs.emplace(name,i);
        return i;
    } else {
        return it->second;
    }
}

NodeType GraphData::Builder::type(const NodeID& id) {
    auto it = mTypes.find(id);
    if(it == mTypes.end()) {
        return NODE;
    }
    return it->second;
}

NodeProperties GraphData::Builder::props(const NodeID &id) {
    auto it = mProperties.find(id);
    if(it == mProperties.end()) {
        return {};
    }
    return it->second;
}

Index GraphData::Builder::index(const NodeID& id) {
    auto it = mIndice.find(id);
    if(it == mIndice.end()) {
        return 0;
    }
    return it->second;
}

Dependencies GraphData::Builder::dependencies(const NodeID& i) {
    auto it = mDependencies.find(i);
    if(it == mDependencies.end()) {
        return {};
    }
    return it->second;
}

void GraphData::Builder::setDependencies(const NodeName& name, const Dependencies &dependencies) {
    mDependencies[id(name)] = dependencies;
}


void GraphData::Builder::setDependencies(const NodeName& name, const NodeNames &dependencies) {
    Dependencies deps;
    Index from = 0;
    Index to = 0;
    for(const NodeName& n : dependencies) {
        deps.push_back(Dependency{id(n),from,to++});
    }
    setDependencies(name,deps);
}

void GraphData::Builder::setProperties(const NodeName& name, const NodeProperties& props) {
    mProperties[id(name)] = props;
}

void GraphData::Builder::addProperty(const NodeName& name,const QString& pname,const QJsonValue& val) {
    properties(name).insert(pname,val);
}

void GraphData::Builder::setNode(const NodeName& name, const Node& node) {
    setProperties(name,node.properties());
    setIoIndex(name,node.IOIndex());
    Dependencies deps;
    for(const Node::Connexion& c : node.fanIn()) {
        deps.push_back(Dependency{id(c.node->name()),c.from,c.to});
    }
    setDependencies(name,deps);
    //setType(name,node.type());
}

QJsonObject& GraphData::Builder::properties(const NodeName& name) {
    auto it = mProperties.find(id(name));
    if(it == mProperties.end()) {
        auto p = mProperties.emplace(id(name),QJsonObject{});
        return p.first->second;
    }
    return it->second;
}

void GraphData::Builder::setType(const NodeName& name, const NodeType& type) {
    NodeID i = id(name);
    mTypes[i] = type;
    if(type == OUTPUT_CLUSTER || type == OUTPUT) {
        mOutputs.push_back(i);
    } else if (type == INPUT_CLUSTER || type == INPUT) {
        mInputs.push_back(i);
    }
}

void GraphData::Builder::setIoIndex(const NodeName& name, const Index& index) {
    mIndice[id(name)] = index;
}

const SharedData GraphData::Builder::build(const QString& filename) {
    NodeDatas dats;
    for(NodeID i = 0; i < mID; i++) {
        dats.emplace_back(i,mNames.at(i),dependencies(i),type(i),props(i),index(i),
                          nodeInputCount(i),nodeOutputCount(i),
                          nodeInputNames(i),nodeOutputNames(i));
    }
    return std::make_shared<GraphData>(dats,filename);
}

const NodeIDs& GraphData::Builder::outputs() const {
    return mOutputs;
}

const NodeIDs& GraphData::Builder::inputs() const {
    return mInputs;
}

const NodeName& GraphData::Builder::name(const NodeID& id) const {
    return mNames.at(id);
}

void GraphData::Builder::setNodeInputNames(const NodeName& name, const Names& names) {
    mNodeInputCount.at(id(name)) = names.size();
    mNodeInputsNames.at(id(name)) = names;
}

void GraphData::Builder::setNodeOutputNames(const NodeName& name, const Names& names) {
    mNodeOutputCount.at(id(name)) = names.size();
    mNodeOutputsNames.at(id(name)) = names;
}

Names GraphData::Builder::nodeInputNames(const NodeID& id) {
    auto it = mNodeInputsNames.find(id);
    if(it == mNodeInputsNames.end()) {
        return {};
    }
    return it->second;
}

Names GraphData::Builder::nodeOutputNames(const NodeID& id) {
    auto it = mNodeOutputsNames.find(id);
    if(it == mNodeOutputsNames.end()) {
        return {};
    }
    return it->second;
}

int GraphData::Builder::nodeInputCount(const NodeID& id) {
    auto it = mNodeInputCount.find(id);
    if(it != mNodeInputCount.end()) {
        return it->second;
    } else {
        return dependencies(id).size(); //Defaults
    }
}

int GraphData::Builder::nodeOutputCount(const NodeID& id) {
    auto it = mNodeOutputCount.find(id);
    if(it != mNodeOutputCount.end()) {
        return it->second;
    } else {
        return type(id) == OUTPUT or type(id) == OUTPUT_CLUSTER ? 0 : 1; //Defaults
    }
}

const NodeDatas& GraphData::nodeDatas() const {
    return mDatas;
}

const QString& GraphData::filename() const {
    return mFilename;
}

QJsonObject GraphData::json() const {
    QJsonObject obj;
    obj.insert("filename",mFilename);
    obj.insert("node_count",(int)mDatas.size());

    using pair_type = NodeDatas::value_type;
    QJsonArray nodes;
    for(const NodeData& d : mDatas) {
        nodes.append(d.json());
    }
    obj.insert("nodes",nodes);
    return obj;
}

}
