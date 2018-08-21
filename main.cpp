#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator t;

    QLocale *lc = new QLocale();

    if (lc->languageToString(lc->language()) == "English") {

        t.load("en.qm", ":/translators");


    } else if (lc->languageToString(lc->language()) == "Chinese") {

        t.load("cn.qm", ":/translators");

    }

    a.installTranslator(&t);

    MainWindow w;
    w.show();

    return a.exec();
}
