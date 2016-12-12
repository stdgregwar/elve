#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include "NodeData.h"

class GraphData;
typedef std::shared_ptr<GraphData> SharedData;

class Graph;
class GraphData
{
    friend class Graph;
public:
    class Builder{
    public:
        Builder();
        void setDependencies(const NodeName& name, const NodeNames& dependencies);
        void setProperties(const NodeName& name, const NodeProperties& props);
        QJsonObject& properties(const NodeName& name);
        void setType(const NodeName& name, const NodeType& type);
        void setIoIndex(const NodeName& name, const Index& index);
        const SharedData build(const QString &filename);
    private:
        NodeID id(const NodeName& name);
        NodeType type(const NodeID& id);
        NodeProperties props(const NodeID& id);
        Index index(const NodeID& id);
        NodeIDs dependencies(const NodeID& id);

        NodeID mID;
        std::unordered_map<NodeID,std::vector<NodeName>> mDependencies;
        std::unordered_map<NodeID,NodeProperties> mProperties;
        std::unordered_map<NodeID,Index> mIndice;
        std::unordered_map<NodeID,NodeType> mTypes;
        std::unordered_map<NodeName,NodeID> mIDs;
        std::unordered_map<NodeID,NodeName> mNames;
    };

    GraphData(const NodeDatas& nodesData = {}, QString filename = "graph");
    GraphData(const QJsonObject& obj);
    const NodeDatas& nodeDatas() const;
    const QString& filename() const;
    QJsonObject json() const;
private:
    QString mFilename;
    NodeDatas mDatas;
};

#endif // GRAPHDATA_H
