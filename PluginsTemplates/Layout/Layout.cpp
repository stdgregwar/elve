#include "Layout.h"

using namespace Elve;

LayoutName::LayoutName() {
    int myOp;
    opts().add_options() // Use boost options to setup plugin command options, see boost_program_option doc
            ("myoption",po::value(&myOp),"an integer option you can set while calling cmd");
}

void LayoutName::setGraph(SharedGraph graph)
{
    // TODO
    // Here make your calls to system()
    // See LevelLayout for a concrete example
}
