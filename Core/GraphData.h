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
        void addNode(const NodeName& name, const NodeType& type = NODE, const NodeNames& dependencies = {}, const NodeProperties& props = {},const Index& ioindex = 0);
        void setDependencies(const NodeName& name, const NodeNames& dependencies);
        void setProperties(const NodeName& name, const NodeProperties& props);
        void setType(const NodeName& name, const NodeType& type);
        void setIoIndex(const NodeName& name, const Index& index);
        SharedData build(QString filename);
    private:
        /*void setDependencies(NodeData& data, const NodeNames& dependencies);
        void setProperties(NodeData& data, const NodeProperties& props);
        void setType(const NodeData& data, const NodeType& type);
        void setIoIndex(const NodeName& data, const Index& index);*/

        std::unordered_map<NodeName,std::vector<NodeName>> mDependencies;
        std::unordered_map<NodeName,NodeProperties> mProperties;
        std::unordered_map<NodeName,Index> mIndice;
        std::unordered_map<NodeName,NodeType> mTypes;
        std::unordered_map<NodeName,NodeID> mIDs;
        std::vector<NodeName> mNames;
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
