#include "NodeData.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QHash>
namespace Elve {
Dependency::Dependency(const NodeID& id) : id(id),from(0), to(0) {}

Dependency::Dependency(const NodeID &id,Index from, Index to) :
    id(id),
    from(from),
    to(to) {}
Dependency::Dependency(const QJsonArray& v) :
    id(v.at(0).toInt()),
    from(v.at(1).toInt()),
    to(v.at(2).toInt()) {}
QJsonValue Dependency::json() const {
    QJsonArray a;
    a.append((int)id);
    a.append((int)from);
    a.append((int)to);
    return a;
}

NodeData::NodeData(const NodeID& id, const NodeName &name, const Dependencies &dependencies, const NodeType& type, const NodeProperties& props, const Index& ioindex, int inputCount, int outputCount, const Names &inputNames, const Names &outputNames)
    : mId(id),
      mName(name),
      mDependencies(dependencies),
      mType(type),
      mProperties(props),
      mIOIndex(ioindex),
      mInputCount(inputCount),
      mOutputCount(outputCount),
      mInputNames(inputNames),
      mOutputNames(outputNames)
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
        mDependencies.push_back(j.toArray());
    }
    QJsonArray inputs = obj.value("inputs").toArray();
    for(const QJsonValue& j : inputs) {
        mInputNames.push_back(j.toString().toStdString());
    }
    mInputCount = mInputNames.size();
    QJsonArray outputs = obj.value("outputs").toArray();
    for(const QJsonValue& j : outputs) {
        mOutputNames.push_back(j.toString().toStdString());
    }
    mOutputCount = mOutputNames.size();
}

const QJsonObject NodeData::json() const {
    static std::unordered_map<NodeType,QString> typeToString{{NODE,"node"},
                                                             {INPUT,"input"},
                                                             {OUTPUT,"output"},
                                                             {CLUSTER,"cluster"}};
    QJsonObject obj;
    obj.insert("properties",properties());
    obj.insert("id",(int)id());
    obj.insert("name",QString::fromStdString(name()));
    obj.insert("type",typeToString.at(type()));
    obj.insert("io_index",(int)ioIndex());


    QJsonArray array;
    for(const Dependency& dep : mDependencies) {
        array.append(dep.json());
    }
    obj.insert("dependencies",array);

    QJsonArray inputs;
    for(int i = 0; i < mInputCount; i++) {
        Name name = inputName(i);
        inputs.append(name.c_str());
    }
    obj.insert("inputs",inputs);

    QJsonArray outputs;
    for(int i = 0; i < mOutputCount; i++) {
        Name name = outputName(i);
        outputs.append(name.c_str());
    }
    obj.insert("outputs",outputs);

    return obj;
}

int NodeData::inputCount() const {
    return mInputCount;
}

int NodeData::outputCount() const {
    return mOutputCount;
}

inline Name base26(int i) {
    static Name alph = "abcdefghijklmnopqrstuvwxyz";
    Name n = i==0 ? "a" : "";
    while(i != 0) {
        n += alph.at(i%26);
        i/=26;
    }
    return n;
}

Name NodeData::inputName(const Index& i) const {
    if(mInputNames.size() > i) {
        return mInputNames.at(i);
    }
    return base26(i);
}

Name NodeData::outputName(const Index& i) const {
    if(mOutputNames.size() > i) {
        return mOutputNames.at(i);
    }
    return base26(i);
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

const Dependencies& NodeData::dependencies() const {
    return mDependencies;
}

const NodeType& NodeData::type() const {
    return mType;
}

const Index& NodeData::ioIndex() const {
    return mIOIndex;
}
}
