#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GraphWidget.h"
#include <ui_mainwindow_test.h>
#include "PluginManager.h"
#include "QConsoleWidget.h"
#include "StoreView.h"


namespace Elve
{

class MainWindow : public QMainWindow, public Singleton<MainWindow>, public GraphWidgetListener
{
    friend class Singleton<MainWindow>;
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    Ui::MainWindow ui;
    void newWindowWithFile(SharedEGraph g, QString filename);
    void connectTab(QMdiSubWindow* tab);
    void runUiCommand(const QString& cmd);
    void runCommandOnShownGraph(const QString& cmd);
    void closeAllTabs();

    void graphChanged(SharedEGraph old, SharedEGraph newg) override;
    void runCommand(const QString& cmd) override;

    GraphWidget* viewport();
    ~MainWindow();
private:
    //PluginManager mPluginManager;
    QMdiSubWindow* mCurrentTab;
    QConsoleWidget* mConsole;
    StoreView* mStoreView;
public slots:

    void onFileOpen(const QString& file);
    void on_export_trigerred(FileExporterPlugin* exp);
    void on_layout_trigerred(LayoutPlugin *layout);
    void on_look_triggered(LookFactoryPlugin* factory);
    void on_import_trigerred(GraphLoaderPlugin* ld);
    void on_transform_triggered(GraphTransformPlugin* trans);
    void on_tab_change(QMdiSubWindow* tab);
    void applyQSSTheme();
private slots:
    void on_group();
    void on_toggle();
    void on_ungroup();
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionSave_triggered();
    void on_actionFit_triggered();
    void on_actionFullscreen_triggered(bool checked);
};

}
#endif // MAINWINDOW_H
