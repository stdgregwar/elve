#include "MainWindow.h"

#include <QApplication>
#include <QOpenGLContext>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QPluginLoader>
#include <QAction>
#include <QJsonDocument>
#include "QConsoleWidget.h"
#include <QMessageBox>

#include <interfaces/GraphLoaderPlugin.h>
#include <Graph.h>
#include <EGraph.h>
#include <QMdiSubWindow>
#include <QMainWindow>
#include <QDockWidget>

#include "FileLoadAction.h"
#include "LayoutLoadAction.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mCurrentTab(nullptr)
{
    Q_INIT_RESOURCE(coreresources); //Init coremodule resources

    PluginManager::get().load("plugins");

    QFile File(":skin/darkorange.stylesheet");
     File.open(QFile::ReadOnly);
     QString StyleSheet = QLatin1String(File.readAll());

     qApp->setStyleSheet(StyleSheet);

    ui.setupUi(this);

    connect(ui.mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(on_tab_change(QMdiSubWindow*)));

    PluginManager& pluginManager = PluginManager::get();

    //setup loaders
    for(auto& l : pluginManager.loaders()) {
        FileLoadAction* a = new FileLoadAction(l,l->formatName(),this);
        connect(a,SIGNAL(triggered(GraphLoaderPlugin*)),this,SLOT(on_import_trigerred(GraphLoaderPlugin*)));
        ui.menuImport->addAction(a);
    }
    //setup layouts
    for(auto& l : pluginManager.layouts()) {
        LayoutLoadAction* a = new LayoutLoadAction(l,l->name(),this);
        connect(a,SIGNAL(triggered(LayoutPluginFactory*)),this,SLOT(on_layout_trigerred(LayoutPluginFactory*)));
        ui.menuLayout->addAction(a);
    }

    //Setup terminal
    QDockWidget* dw = new QDockWidget("Shell",this);
    mConsole = new QConsoleWidget(this);
    dw->setWidget(mConsole);
    addDockWidget(Qt::BottomDockWidgetArea,dw);
    setDockOptions(QMainWindow::DockOption::AnimatedDocks);
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
        g->setFilename(filename.toStdString());
        newWindowWithFile(EGraph::fromGraph(g),filename);
    }
}

void MainWindow::on_layout_trigerred(LayoutPluginFactory* layout) {
    qDebug() << "Setting layout to " + layout->name();
    GraphWidget* vp = viewport();
    if(vp) {
        vp->setLayout(layout->create());
    }
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
        //mViewport->fromJson(doc.object());
    } else {
        throw std::runtime_error("Couldn't open file " + filename.toStdString() + " for reading.");
    }
}

void MainWindow::connectTab(QMdiSubWindow* tab) {
    mCurrentTab = tab;
    GraphWidget* gw = viewport();
    if(!gw) {
        qDebug() << "Could not cast tab to viewport. Exiting";
        //qApp->quit();
        return;
    }
    connect(ui.actionBorder,SIGNAL(triggered()),gw,SLOT(borderSelect()));
    mCurrentTab = tab;
}

void MainWindow::disconnectTab(QMdiSubWindow* tab) {
    GraphWidget* gw = viewport();
    if(!gw) {
        return;
    }
    disconnect(ui.actionBorder,SIGNAL(triggered()),gw,SLOT(borderSelect()));
}

void MainWindow::on_tab_change(QMdiSubWindow* tab) {
    disconnectTab(mCurrentTab);
    connectTab(tab);
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit(); //TODO last chance save
}

GraphWidget* MainWindow::viewport() {
    if(mCurrentTab) {
        QMainWindow* mw = dynamic_cast<QMainWindow*>(mCurrentTab->widget());
        return dynamic_cast<GraphWidget*>(mw->centralWidget());
    }
    return nullptr;
}

void MainWindow::newWindowWithFile(SharedEGraph g, QString filename) {
    GraphWidget* gw = new GraphWidget(this,filename.split("/").last());
    QMainWindow* cw = new QMainWindow(ui.mdiArea);

    cw->setCentralWidget(gw);

    /*QDockWidget* dw = new QDockWidget("Shell",cw);

    dw->setWidget(new QConsoleWidget(dw));
    cw->addDockWidget(Qt::BottomDockWidgetArea,dw);*/

    QMdiSubWindow* w = ui.mdiArea->addSubWindow(cw);
    w->setWindowTitle(filename);
    w->setWindowState(Qt::WindowMaximized);
    w->setAttribute(Qt::WA_DeleteOnClose);
    gw->setGraph(g);
}

void MainWindow::on_actionSave_triggered()
{
    const SharedEGraph graph = viewport()->graph();
    if(graph) {
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
        try {
            graph->toFile(filename);
        } catch(std::exception e) {
            QMessageBox::critical(this,"Error", e.what());
        }
    } else {
        QMessageBox::information(this,"Mhhh...","There is no current graph to save.");
    }
}
