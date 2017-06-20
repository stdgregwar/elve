#pragma once

#include <LayoutPlugin.h>

/**
 * @brief Layouts are producer classes that build a physical system based on graph topography
 *
 *
 * The graph topography is usually iterated on and the layout plugin build the physical system
 * the built-in physic engine can be accesed with the Elve::LayoutPlugin::system() inherited method.
 *
 * If your layout does not need a physical system or the actual system does not suits your needs
 * you are free to override Elve::LayoutPlugin::tick to implement your own stuff.
 */
class LayoutName : public Elve::LayoutPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.epfl.lap.LayoutName")
    Q_INTERFACES(Elve::LayoutPlugin)

    ELVE_LAYOUT(LayoutName,"Layout name in GUI","layout_name_for_cli")
    LayoutName();
    void setGraph(Elve::SharedGraph g) override;
};
