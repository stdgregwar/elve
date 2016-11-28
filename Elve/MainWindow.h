#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GraphWidget.h"
#include <ui_mainwindow_test.h>
#include "PluginManager.h"
#include "QConsoleWidget.h"

class MainWindow : public QMainWindow, public Singleton<MainWindow>
{
    friend class Singleton<MainWindow>;
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    Ui::MainWindow ui;
    void newWindowWithFile(SharedEGraph g, QString filename);
    void connectTab(QMdiSubWindow* tab);
    void disconnectTab(QMdiSubWindow* tab);
    GraphWidget* viewport();
    ~MainWindow();
private:
    //PluginManager mPluginManager;
    QMdiSubWindow* mCurrentTab;
    QConsoleWidget* mConsole;
public slots:
    void onFileOpen(const QString& file);
    void on_import_trigerred(GraphLoaderPlugin* ld);
    void on_layout_trigerred(LayoutPlugin *layout);
    void on_tab_change(QMdiSubWindow* tab);
private slots:
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionSave_triggered();
    void on_actionFit_triggered();
};

#endif // MAINWINDOW_H
