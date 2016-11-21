#ifndef LAYOUTPLUGINFACTORY_H
#define LAYOUTPLUGINFACTORY_H

#include <QString>
#include <QGenericPlugin>

#define LayoutPluginFactory_iid "ch.epfl.lap.elve.LayoutPluginFactory"

class LayoutPlugin;

class LayoutPluginFactory : public QObject
{
    Q_OBJECT
public:
    LayoutPluginFactory();
    virtual LayoutPlugin* create() = 0;
    virtual QString name() = 0;
    virtual QString cliName() = 0;
};

Q_DECLARE_INTERFACE(LayoutPluginFactory,LayoutPluginFactory_iid)

//Macro to export LayoutPlugin as a factory
#define ELVE_EXPORT_LAYOUT(Layout,full_name,cli_name) class Layout ## Factory : public LayoutPluginFactory\
{\
    Q_OBJECT\
    Q_PLUGIN_METADATA(IID "ch.epfl.lap." cli_name)\
    Q_INTERFACES(LayoutPluginFactory)\
    public:\
    Layout ## Factory(){}\
    LayoutPlugin* create() override {\
        return new Layout();\
    }\
    QString name() override {return (full_name);}\
    QString cliName() override {return (cli_name);}\
};

#endif // LAYOUTPLUGINFACTORY_H
