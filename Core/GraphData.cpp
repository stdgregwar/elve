#include "GraphData.h"
#include <QJsonObject>
#include <QJsonArray>

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
        mDatas.emplace(
                    (j.toObject().value("id").toString("unnamed").toStdString()),
                    (j.toObject())
                    );
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
    for(const pair_type& p : mDatas) {
        nodes.append(p.second.json());
    }
    obj.insert("nodes",nodes);
    return obj;
}
