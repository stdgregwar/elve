# Plugin Templates

This folder contains a template for kind of plugin there is

## Setup a new plugin

To setup a new plugin, copy the desired template to the Plugins directory. Rename the folder, the .pro and both source file to fit your plugin name
add a line in Plugins.pro to tell qmake your Plugin exist. You must then edit the .pro of your plugin.

- Uncomment the DESTDIR line
- Change the TARGET name
- Edit HEADERS and SOURCES to link your renamed files

You can then follow instructions to write your plugin from the documentation
