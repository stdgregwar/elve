#include "GraphData.h"

GraphData::GraphData(const NodeDatas& nodesData,QString filename)
    : mDatas(nodesData), mFilename(filename)
{

}

const NodeDatas& GraphData::nodeDatas() const {
    return mDatas;
}
const QString& GraphData::filename() const {
    return mFilename;
}
