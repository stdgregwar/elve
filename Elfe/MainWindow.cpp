#include "MainWindow.h"

#include <QApplication>
#include <QOpenGLContext>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QPluginLoader>
#include <QAction>
#include <QJsonDocument>
#include <QMessageBox>

#include <interfaces/GraphLoaderPlugin.h>
#include <Graph.h>
#include <QMdiSubWindow>

#include "FileLoadAction.h"
#include "LayoutLoadAction.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mPluginManager("plugins")
{
    ui.setupUi(this);
    //setCentralWidget(mViewport);

    //loadBlif("mul5.blif");

    connect(ui.actionBorder,SIGNAL(triggered()),mViewport,SLOT(borderSelect()));

    //setup loaders
    for(auto& l : mPluginManager.loaders()) {
        FileLoadAction* a = new FileLoadAction(l,l->formatName(),this);
        connect(a,SIGNAL(triggered(GraphLoaderPlugin*)),this,SLOT(on_import_trigerred(GraphLoaderPlugin*)));
        ui.menuImport->addAction(a);
    }
    //setup layouts
    for(auto& l : mPluginManager.layouts()) {
        LayoutLoadAction* a = new LayoutLoadAction(l,l->layoutName(),this);
        connect(a,SIGNAL(triggered(LayoutPlugin*)),this,SLOT(on_layout_trigerred(LayoutPlugin*)));
        ui.menuLayout->addAction(a);
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_import_trigerred(GraphLoaderPlugin* ld) {
    QString filename = QFileDialog::getOpenFileName(this,"Open " + ld->formatName(),"",ld->fileFilter());
    if(filename != "") {
        SharedGraph g;
        try {
            g = ld->load(filename);
        } catch (std::exception e) {
            QMessageBox::critical(this,"Error", e.what());
        }
        g->setFilename(filename);
        newWindowWithFile(g,filename);
        //mViewport->setGraph(g);
    }
}

void MainWindow::on_layout_trigerred(LayoutPlugin* layout) {
    qDebug() << "Setting layout to " + layout->layoutName();
    mViewport->setLayout(layout);
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this,"Save visualization");
    dialog.setNameFilters({"ELFE files (*.json *.elfe)"});
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if(dialog.exec()) {
        QStringList list = dialog.selectedFiles();
        if(list.length() != 1) {
            return;
        }
        if(QFileInfo(list.last()).isDir()) {
            return;
        }
        onFileOpen(list.first());
    }
}

void MainWindow::onFileOpen(const QString& filename){
    QString ext = filename.split(".").last();
    QFile file(filename);
    if(file.open(QFile::ReadOnly)) {
        QJsonDocument doc;
        if(ext.toLower() == "elfe") {
            doc = QJsonDocument::fromBinaryData(file.readAll());
        } else { //Assume it's json
            doc = QJsonDocument::fromJson(file.readAll());
        }
        mViewport->fromJson(doc.object());
    } else {
        throw std::runtime_error("Couldn't open file " + filename.toStdString() + " for reading.");
    }
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit(); //TODO last chance save
}

void MainWindow::newWindowWithFile(SharedGraph g, QString filename) {
    GraphWidget* gw = new GraphWidget(this,filename.split("/").last());
    QMdiSubWindow* w = ui.mdiArea->addSubWindow(gw);
    w->setWindowState(Qt::WindowMaximized);
    w->setAttribute(Qt::WA_DeleteOnClose);
    gw->setGraph(g);
}

void MainWindow::on_actionSave_triggered()
{
    const SharedGraph graph = mViewport->graph();
    if(graph) {
        QJsonObject json = mViewport->json();
        QFileDialog dialog(this,"Save visualization");
        dialog.setNameFilters({"ELFE json (*.json)","ELFE bin (*.elfe)"});
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.exec();
        QString selectedExt = dialog.selectedNameFilter().split(".").last();
        selectedExt.remove(")");
        dialog.setDefaultSuffix(selectedExt);
        QStringList list = dialog.selectedFiles();
        if(list.length() != 1) {
            return;
        }
        QString filename = list.first();
        QString suffix = filename.split(".").last();
        QJsonDocument doc(json);
        try {
            QFile file(filename);
            if(file.open(QFile::WriteOnly)) {
                if(suffix == "elfe") {
                    file.write(doc.toBinaryData());
                } else {
                    file.write(doc.toJson());
                }
                file.close();
            } else {
                throw std::runtime_error("Couldn't write to file " + filename.toStdString());
            }
        } catch(std::exception e) {
            QMessageBox::critical(this,"Error", e.what());
        }
    } else {
        QMessageBox::information(this,"Mhhh...","There is no current graph to save.");
    }
}
