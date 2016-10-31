#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GraphWidget.h"
#include <ui_mainwindow_test.h>
#include "PluginManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    Ui::MainWindow ui;
    ~MainWindow();
private:
    GraphWidget* mViewport;
    PluginManager mPluginManager;
public slots:
    void onFileOpen(const QString& file);
    void on_import_trigerred(GraphLoaderPlugin* ld);
    void on_layout_trigerred(LayoutPlugin* layout);
private slots:
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
