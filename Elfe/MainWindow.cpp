#include "MainWindow.h"
#include "GraphWidget.h"
#include <QApplication>
#include <QOpenGLContext>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QPluginLoader>
#include "Graph.h"
#include "BlifLoader.h"
#include "interfaces/LoaderInterface.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    mViewport = new GraphWidget();
    setCentralWidget(mViewport);
    //loadBlif("mul5.blif");

    connect(ui.actionBorder,SIGNAL(triggered()),mViewport,SLOT(borderSelect()));
    connect(ui.actionLevelLayout,SIGNAL(triggered()),mViewport,SLOT(setLevelLayout()));
    connect(ui.actionSimpleLayout,SIGNAL(triggered()),mViewport,SLOT(setSimpleLayout()));

    //test loading of loaders plugins
    qDebug() << "Trying to load";
    QDir dir("plugins/loaders");
    for(const QFileInfo& info : dir.entryInfoList(QDir::Files)) {
        qDebug() << "Trying to load" << info.baseName();
        QPluginLoader* qpl = new QPluginLoader(info.filePath(),this);
        if(!qpl->load()) {
            qDebug() << qpl->errorString() << endl;
        } else {
            QObject* obj = qpl->instance();
            LoaderInterface* interface = qobject_cast<LoaderInterface*>(obj);
            if(interface) {
                qDebug() << "plugin is a loader!";
                SharedGraph g = interface->load("mul5.blif");
                mViewport->setGraph(g);
            }
        }
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_actionBlif_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "$HOME", tr("Blif Files (*.blif)"));
    loadBlif(fileName);
}


void MainWindow::loadBlif(const QString& filename) {
    BlifLoader* bl = new BlifLoader(filename.toStdString());
    SharedGraph g = bl->load();
    mViewport->setGraph(g);
}
