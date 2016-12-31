#ifndef GRAPHTRANSFORM_H
#define GRAPHTRANSFORM_H

#include <EGraph.h>

class GraphTransform
{
public:
    virtual SharedEGraph transform(const SharedEGraph& graph) = 0;
};

#endif // GRAPHTRANSFORM_H
