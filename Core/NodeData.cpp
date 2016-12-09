#include "NodeData.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QHash>

NodeData::NodeData(const NodeID& id, const NodeName &name, const NodeIDs& dependencies, const NodeType& type, const NodeProperties& props, const Index& ioindex)
    : mId(id),
      mDependencies(dependencies),
      mType(type),
      mProperties(props),
      mIOIndex(ioindex),
      mName(name)
{

}

const NodeName& NodeData::name() const {
    return mName;
}

NodeData::NodeData(const QJsonObject& obj)
{
    static QHash<QString,NodeType> stringToType{{"node",NODE},
                                                             {"input",INPUT},
                                                             {"output",OUTPUT},
                                                             {"cluster",CLUSTER}};


    mProperties = obj.value("properties").toObject();
    mId = obj.value("id").toInt(-1);
    mName = obj.value("name").toString("unnamed").toStdString();
    mType = stringToType.value(obj.value("type").toString("node"));
    mIOIndex = obj.value("io_index").toInt(0);
    QJsonArray arr = obj.value("dependencies").toArray();
    for(const QJsonValue& j : arr) {
        mDependencies.push_back(j.toInt(-1));
    }
}

const QJsonObject NodeData::json() const {
    static std::unordered_map<NodeType,QString> typeToString{{NODE,"node"},
                                                             {INPUT,"input"},
                                                             {OUTPUT,"output"},
                                                             {CLUSTER,"cluster"}};
    QJsonObject obj;
    obj.insert("properties",properties());
    obj.insert("id",id());
    obj.insert("name",QString::fromStdString(name()));
    obj.insert("type",typeToString.at(type()));
    obj.insert("io_index",(int)ioIndex());


    QJsonArray array;
    for(const NodeID& id : mDependencies) {
        array.append(id);
    }
    obj.insert("dependencies",array);
    return obj;
}



NodeProperties &NodeData::properties() {
    return mProperties;
}

const NodeProperties &NodeData::properties() const {
    return mProperties;
}

const NodeID& NodeData::id() const {
    return mId;
}

const NodeIDs& NodeData::dependencies() const {
    return mDependencies;
}

const NodeType& NodeData::type() const {
    return mType;
}

const Index& NodeData::ioIndex() const {
    return mIOIndex;
}
