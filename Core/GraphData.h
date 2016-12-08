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
    GraphData(const NodeDatas& nodesData = {}, QString filename = "graph");
    const NodeDatas& nodeDatas() const;
    const QString& filename() const;
private:
    QString mFilename;
    NodeDatas mDatas;
};

#endif // GRAPHDATA_H
