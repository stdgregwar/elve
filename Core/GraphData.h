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
    GraphData();
private:
    QString mFilename;
    NodeDatas mDatas;
};

#endif // GRAPHDATA_H
