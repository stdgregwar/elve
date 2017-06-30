#include "MainWindow.h"

#include <iostream>
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
#include <QWhatsThis>

#include <interfaces/GraphLoaderPlugin.h>
#include <Graph.h>
#include <ExtendedGraph.h>
#include <QMdiSubWindow>
#include <QMainWindow>
#include <QDockWidget>
#include <QShortcut>

#include "FileLoadAction.h"
#include "FileExportAction.h"
#include "LayoutLoadAction.h"
#include "TransformAction.h"
#include "CommandLine.h"
#include "LookLoadAction.h"


namespace Elve {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mCurrentTab(nullptr)
{
    //Init coremodule resources

    PluginManager::get().load("plugins");
    PluginManager::get().load(".elve/plugins");

    QObject::connect(new QShortcut(QKeySequence("F12"), this), SIGNAL(activated()), this, SLOT(applyQSSTheme()));
    applyQSSTheme();

    ui.setupUi(this);

    connect(ui.mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(on_tab_change(QMdiSubWindow*)));

    PluginManager& pluginManager = PluginManager::get();

    //setup loaders
    for(auto& l : pluginManager.loaders()) {
        FileLoadAction* a = new FileLoadAction(l,l->formatName(),this);
        connect(a,SIGNAL(triggered(GraphLoaderPlugin*)),this,SLOT(on_import_trigerred(GraphLoaderPlugin*)));
        ui.menuImport->addAction(a);
    }

    //setup exporters
    for(auto& l : pluginManager.exporters()) {
        FileExportAction* a = new FileExportAction(l,l->formatName(),this);
        connect(a,SIGNAL(triggered(FileExporterPlugin*)),this,SLOT(on_export_trigerred(FileExporterPlugin*)));
        ui.menuExport->addAction(a);
    }

    //setup layouts
    for(auto& l : pluginManager.layouts()) {
        LayoutLoadAction* a = new LayoutLoadAction(l,l->name(),this);
        connect(a,SIGNAL(triggered(LayoutPlugin*)),this,SLOT(on_layout_trigerred(LayoutPlugin*)));
        ui.menuLayout->addAction(a);
    }

    //Setup transforms
    for(auto& l : pluginManager.transforms()) {
        TransformAction* a = new TransformAction(l,l->name(),this);
        connect(a,SIGNAL(triggered(GraphTransformPlugin*)),this,SLOT(on_transform_triggered(GraphTransformPlugin*)));
        switch(l->type()) {
        case SELECTION:
            ui.menuSelect->addAction(a);
            break;
        default: //TODO add more types
            ui.menuTransform->addAction(a); break;
        }
    }

    //Setup looks
    for(auto& l : pluginManager.looks()) {
        LookLoadAction* a = new LookLoadAction(l,l->lookName(),this);
        connect(a,SIGNAL(triggered(LookFactoryPlugin*)),this,SLOT(on_look_triggered(LookFactoryPlugin*)));
        ui.menuLook->addAction(a);
    }

    ui.menuHelp->addAction(QWhatsThis::createAction(this));

    //Setup terminal
    QDockWidget* dw = new QDockWidget("Shell",this);
    mConsole = new QConsoleWidget(this);
    dw->setWidget(mConsole);
    dw->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::BottomDockWidgetArea,dw);
    setDockOptions(QMainWindow::DockOption::AnimatedDocks);

    //Setup store view
    QDockWidget* vdw = new QDockWidget("Store",this);
    mStoreView = new StoreView(CommandLine::get().store(),this);
    vdw->setWidget(mStoreView);
    vdw->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea,vdw);
    setDockOptions(QMainWindow::DockOption::AnimatedDocks);
}



MainWindow::~MainWindow()
{

}

void MainWindow::closeAllTabs() {
    ui.mdiArea->closeAllSubWindows();
}

void MainWindow::on_import_trigerred(GraphLoaderPlugin* ld) {
    QString filename = QFileDialog::getOpenFileName(this,"Open " + ld->formatName(),"",ld->fileFilter());
    if(filename != "") {
        runUiCommand(QString("load_%1 \"%2\"").arg(ld->cliName().c_str(),filename));
        /*SharedGraph g;
        //try {
            g = ld->load(filename);
        //} catch (std::exception e) {
            //QMessageBox::critical(this,"Error", e.what());
        //}
        //g->setFilename(filename.toStdString());
        SharedEGraph eg = EGraph::fromGraph(g);
        CommandLine::get().store().push(eg);
        newWindowWithFile(eg,filename);*/
    }
}

void MainWindow::on_transform_triggered(GraphTransformPlugin* trans) {
    runCommandOnShownGraph((trans->cliName() + " -n").c_str());
}

void MainWindow::on_export_trigerred(FileExporterPlugin* exp) {
    QString filename = QFileDialog::getSaveFileName(this,"Export " + exp->formatName(),"",exp->fileFilter());
    QString ext = filename.split(".").last();
    QString true_ext = QString::fromStdString(exp->cliName()); //TODO fix
    if(ext != true_ext) {
        filename+="."+true_ext;
    }
    if(filename != "") {
        runCommandOnShownGraph(QString("save_%1 \"%2\"").arg(exp->cliName().c_str(),filename));
    }
}

void MainWindow::runUiCommand(const QString& cmd) {
    mConsole->run_command(cmd,true);
}

void MainWindow::on_layout_trigerred(LayoutPlugin* layout) {
    if(CommandLine::get().store().empty()) return;
    layout->uiStart(this,CommandLine::get().store().current());
    //runCommandOnShownGraph(QString("%1_layout").arg(QString::fromStdString(layout->cliName())));
}

void MainWindow::on_look_triggered(LookFactoryPlugin* factory) {
    qDebug() << "setting look to" << factory->lookName();
    runCommandOnShownGraph(QString("%1_look").arg(QString::fromStdString(factory->cliName())));
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this,"Save visualization");
    dialog.setNameFilters({"ELVE files (*.json *.elve)"});
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
        //onFileOpen(list.first());
        runUiCommand(QString("read_graph \"%1\"").arg(list.last()));
    }
}

void MainWindow::onFileOpen(const QString& filename){
    QString ext = filename.split(".").last();
    QFile file(filename);
    if(file.open(QFile::ReadOnly)) {
        QJsonDocument doc;
        if(ext.toLower() == "elve") {
            doc = QJsonDocument::fromBinaryData(file.readAll());
        } else { //Assume it's json
            doc = QJsonDocument::fromJson(file.readAll());
        }
        SharedEGraph eg = ExtendedGraph::fromJSON(doc.object());
        CommandLine::get().store().push(eg);
        newWindowWithFile(eg,filename);
    } else {
        throw std::runtime_error("Couldn't open file " + filename.toStdString() + " for reading.");
    }
}

void MainWindow::connectTab(QMdiSubWindow* tab) {

    if(mCurrentTab && mCurrentTab != tab) {
        //TODO disconnect
        QMainWindow* mw = dynamic_cast<QMainWindow*>(mCurrentTab->widget());
        GraphWidget* gw = mw ? dynamic_cast<GraphWidget*>(mw->centralWidget()) : nullptr;
        gw->stop();
        disconnect(ui.actionRectangle,0,gw,0);
        disconnect(ui.actionToggle,0,gw,0);
        disconnect(ui.actionGroup,0,gw,0);
        disconnect(ui.actionUngroup,0,gw,0);
        disconnect(ui.actionPause_Simulation,0,gw,0);
        disconnect(ui.actionQuicksim,0,gw,0);
    }
    if(mCurrentTab == tab) {
        return;
    }

    mCurrentTab = tab;

    GraphWidget* gw = viewport();
    if(!gw) return;

    connect(ui.actionRectangle,SIGNAL(triggered()),gw,SLOT(borderSelect()));
    connect(ui.actionToggle,SIGNAL(triggered()),gw,SLOT(toggleSelection()));
    connect(ui.actionGroup,SIGNAL(triggered()),gw,SLOT(group()));
    connect(ui.actionUngroup,SIGNAL(triggered()),gw,SLOT(ungroup()));
    connect(ui.actionPause_Simulation,SIGNAL(triggered(bool)),gw,SLOT(toggleSim(bool)));
    connect(ui.actionQuicksim,SIGNAL(triggered()),gw,SLOT(quickSim()));
    gw->start();
    mCurrentTab = tab;
}

void MainWindow::on_group() {
    runCommandOnShownGraph(QString("group %1")
                           .arg(QString::number(
                                    viewport()->graph()->mask())));
}

void MainWindow::on_toggle() {
    runCommandOnShownGraph(QString("select -a %1")
                           .arg(QString::number(
                                    viewport()->graph()->mask())));
}

void MainWindow::on_ungroup() {
    runCommandOnShownGraph(QString("ungroup %1")
                           .arg(QString::number(
                                    viewport()->graph()->mask())));
}



void MainWindow::on_tab_change(QMdiSubWindow* tab) {
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
    GraphWidget* gw = new GraphWidget(this,this);
    QMainWindow* cw = new QMainWindow(ui.mdiArea);

    cw->setCentralWidget(gw);

    /*QDockWidget* dw = new QDockWidget("Shell",cw);

    dw->setWidget(new QConsoleWidget(dw));
    cw->addDockWidget(Qt::BottomDockWidgetArea,dw);*/

    QMdiSubWindow* w = ui.mdiArea->addSubWindow(cw);
    w->setWindowTitle(filename);
    w->setWindowState(Qt::WindowMaximized);
    w->setAttribute(Qt::WA_DeleteOnClose);
    if(g) {
        gw->setGraph(g);
        gw->fit();
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(viewport()) {
        const SharedEGraph graph = viewport()->graph();
        QFileDialog dialog(this,"Save visualization");
        dialog.setNameFilters({"ELVE json (*.json)","ELVE bin (*.elve)"});
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
        //try {
        graph->toFile(filename);
        //} catch(std::exception e) {
        //    QMessageBox::critical(this,"Error", e.what());
        //}
    } else {
        QMessageBox::information(this,"Mhhh...","There is no current graph to save.");
    }
}

void MainWindow::applyQSSTheme() {
    QFile File(":skin/darkorange.css");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    qApp->setStyleSheet(StyleSheet);
}

void MainWindow::graphChanged(SharedEGraph old, SharedEGraph newg) {
    CommandLine::get().graphChanged(old,newg);
}

void MainWindow::runCommand(const QString& cmd) {
    runCommandOnShownGraph(cmd);
}

void MainWindow::runCommandOnShownGraph(const QString& cmd) {
    if(viewport()) {
        SharedEGraph eg = viewport()->graph();
        Store& st = CommandLine::get().store();
        for(int i = 0; i < st.data().size(); i ++) {
            if(st.data()[i] == eg) {
                st.set_current_index(i);
                break;
            }
        }
    }
    runUiCommand(cmd);
}

void MainWindow::on_actionFit_triggered()
{
    if(viewport()) {
        viewport()->fit();
    }
}

}

void Elve::MainWindow::on_actionFullscreen_triggered(bool checked)
{
    checked ? showFullScreen() : showNormal();
}
