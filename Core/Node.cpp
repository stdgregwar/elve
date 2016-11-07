#include "Node.h"
#include <algorithm>
#include <QJsonArray>
#include <QHash>
#include <Graph.h>

Node::Node(const NodeID& id, Type type, Index ioi) : mID(id), mType(type), mIOindex(ioi), mLevel(-1)
{

}

Node::Description::Description(const QJsonObject& obj) {
    static QHash<QString,Type> stringToType{{"node",NODE},
                                                             {"input",INPUT},
                                                             {"output",OUTPUT},
                                                             {"cluster",CLUSTER}};


    properties = obj.value("properties").toObject();
    id = obj.value("id").toString().toStdString();
    type = stringToType.value(obj.value("type").toString());
    ioi = obj.value("io_index").toInt(0);
    if(obj.contains("graph")) { //Parse internal graph
        graph = Graph::fromJson(obj.value("graph").toObject());
    }
}

Node::Node(const Description& desc) :
    mIOindex(desc.ioi),
    mID(desc.id),
    mProperties(desc.properties),
    mType(desc.type),
    mGraph(desc.graph),
    mLevel(-1)
{

}

void Node::addChild(Node* child)
{
    child->_addAncestor(this);
    _addChild(child);
}


void Node::addAncestor(Node* anc)
{
    anc->_addChild(this);
    _addAncestor(anc);
}

void Node::_addChild(Node* child)
{
    mChildren.push_back(child);
}

size_t Node::ancestorCount() const {
    return mAncestors.size();
}

size_t Node::childCount() const
{
    return mChildren.size();
}

void Node::setIOIndex(const Index& i)
{
    mIOindex = i;
}

const Index& Node::IOIndex() const
{
    return mIOindex;
}

void Node::_addAncestor(Node* anc)
{
    mAncestors.push_back(anc);
}

const Node::Ancestors& Node::ancestors() const
{
    return mAncestors;
}

bool Node::isInput() const
{
    return mType == INPUT;
}

bool Node::isOutput() const
{
    return mType == OUTPUT;
}

const Node::Type& Node::type() const
{
    return mType;
}

const NodeLevel& Node::level() const { //TODO add special treatment for OUTPUTS
    if(mType == INPUT) { //Base case
        return mLevel = 0;
    }
    if(mLevel == -1) { //Recursive def
        if(mAncestors.size()) {
            mLevel = (*std::max_element(mAncestors.begin(),mAncestors.end(),
                [](const Node* a,const Node* b) {
                    return a->level() < b->level();
            }))->level() + 1;
        } else {
            mLevel = 0;
        }
    }
    return mLevel;
}

QJsonObject Node::json() const
{
    static std::unordered_map<Type,QString> typeToString{{NODE,"node"},
                                                             {INPUT,"input"},
                                                             {OUTPUT,"output"},
                                                             {CLUSTER,"cluster"}};


    QJsonObject obj;
    obj.insert("properties",mProperties);
    obj.insert("id",QString::fromStdString(id()));
    obj.insert("type",typeToString.at(type()));
    if(isInput() or isOutput()) {
        obj.insert("io_index",(int)IOIndex());
    }

    if(mGraph) { //Node is a cluster and has internal graph
        obj.insert("graph",mGraph->json());
    }

    QJsonArray array;
    for(const Node* n : mAncestors) {
        array.append(QString::fromStdString(n->id()));
    }
    obj.insert("ancestors",array);
    return obj;
}

const QJsonObject& Node::properties() const {
    return mProperties;
}

const Node::Children& Node::children() const
{
    return mChildren;
}

