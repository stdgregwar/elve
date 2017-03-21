TEMPLATE = subdirs

CONFIG += c++11

SUBDIRS += \
    Core \
    Elve \
    Plugins

Plugins.depends = Core
Elve.depends = Core
