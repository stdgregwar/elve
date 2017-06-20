#include "Transform.h"
#include <GraphWidget.h>
#include <Graph.h>
#include <QDebug>
#include <Node.h>

using namespace Elve;

using namespace std;

TransformName::TransformName()
{
    int myOp;
    opts().add_options() //Use boost options to setup plugin command options, see boost_program_option doc
            ("myoption",po::value(&myOp),"an integer option you can set while calling cmd");
}

SharedEGraph TransformName::transform(const SharedEGraph& eg) {
    ///ADD YOUR CODE HERE
    return eg->clone(eg->graph()); //Identity transform
}
