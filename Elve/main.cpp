#include "MainWindow.h" //Main app from Core lib
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(coreresources);
    Elve::MainWindow w;
    w.show();
    return a.exec();
}
