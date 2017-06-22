#include "LookFactoryPlugin.h"

namespace Elve {

LookFactoryPlugin::LookFactoryPlugin()
{
}

LookFactoryPlugin::LookFactoryPlugin(const LookFactoryPlugin& other) :
    QObject(other.parent())
{
}

OrientationHint LookFactoryPlugin::orientationHint() const {
    return BOTTOMUP;
}

}
