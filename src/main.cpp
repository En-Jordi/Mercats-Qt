#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle(QString(titolPrograma_NOM) + "  " +  QString(titolPrograma_VERSIO) + "  " + QString(titolPrograma_RELEASE));

    w.showMaximized();
    w.show();
    return a.exec();
}
