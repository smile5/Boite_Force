#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(640,480);
    w.show();
    w.setWindowIcon(QIcon(":/flambagestypes.gif"));
   // w.setWindowTitle("ForceBox Enise");
    return a.exec();
}
