#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator t;

    t.load("cn.qm", ":/translators");

    a.installTranslator(&t);

    MainWindow w;
    w.show();

    return a.exec();
}
