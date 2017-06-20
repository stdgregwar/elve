TEMPLATE = subdirs

CONFIG += c++11

SUBDIRS += \
    Core \
    Elve \
    Plugins \
    PluginsTemplates

Plugins.depends = Core
PluginsTemplates.depends = Core
Elve.depends = Core
