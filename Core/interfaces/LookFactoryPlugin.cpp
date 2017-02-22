#include "LookFactoryPlugin.h"

LookFactoryPlugin::LookFactoryPlugin()
{
    updateTransforms();
}

LookFactoryPlugin::LookFactoryPlugin(const LookFactoryPlugin& other) :
    QObject(other.parent())
{
    updateTransforms();
}

OrientationHint LookFactoryPlugin::orientationHint() const {
    return BOTTOMUP;
}

NodeLook* LookFactoryPlugin::getNode(const Node& n) {
    NodeLook* nl = node(n);
    nl->setOrientationTransform(&mOrientationTransform,&mInverseOrientationTransform);
}

EdgeLook* LookFactoryPlugin::getEdge(const NodeLook& ancestor, const NodeLook& children) {
    return edge(ancestor,children);
}

void LookFactoryPlugin::updateTransforms() {
    switch(orientationHint()) {
    case TOPDOWN:
        mOrientationTransform = QTransform(-1,0,0, //Invert x coordinates
                                           0,1,0,
                                           0,0,1);
        break;
    case RIGHTLEFT:
        mOrientationTransform = QTransform(0,-1,0, //Flip x,y coordinates
                                           1,0,0,
                                           0,0,1);
        break;
    case LEFTRIGHT:
        mOrientationTransform = QTransform(0,1,0, //Flip and invert x
                                           -1,0,0,
                                           0,0,1);
        break;
    default:
        mOrientationTransform = QTransform();
    }
    mInverseOrientationTransform = mOrientationTransform.inverted();
}
