#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GraphWidget.h"
#include <ui_mainwindow_test.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    Ui::MainWindow ui;
    ~MainWindow();
private:
    GraphWidget* mViewport;
    void loadBlif(const QString& filename);
private slots:
    void on_actionBlif_triggered();
};

#endif // MAINWINDOW_H
