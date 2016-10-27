#include "MainWindow.h"

#include <QApplication>
#include <QOpenGLContext>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QPluginLoader>
#include <QAction>

#include <interfaces/GraphLoader.h>
#include <Graph.h>

#include "FileLoadAction.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mPluginManager("plugins")
{
    ui.setupUi(this);
    mViewport = new GraphWidget();
    setCentralWidget(mViewport);
    //loadBlif("mul5.blif");

    connect(ui.actionBorder,SIGNAL(triggered()),mViewport,SLOT(borderSelect()));
    connect(ui.actionLevelLayout,SIGNAL(triggered()),mViewport,SLOT(setLevelLayout()));
    connect(ui.actionSimpleLayout,SIGNAL(triggered()),mViewport,SLOT(setSimpleLayout()));

    //setup loaders
    for(auto& l : mPluginManager.loaders()) {
        FileLoadAction* a = new FileLoadAction(l,l->formatName(),this);
        connect(a,SIGNAL(triggered(GraphLoader*)),this,SLOT(on_import_trigerred(GraphLoader*)));
        ui.menuImport->addAction(a);
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_import_trigerred(GraphLoader* ld) {
    QString filename = QFileDialog::getOpenFileName(this,"Open " + ld->formatName(),"",ld->fileFilter());
    if(filename != "") {
        SharedGraph g;
        try {
           g = ld->load(filename);
        } catch (std::exception e) {
            QMessageBox("Error", e.what(),QMessageBox::Critical,0,0,0).exec();
        }
        mViewport->setGraph(g);
    }
}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::onFileOpen(const QString& file){

}
