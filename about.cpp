#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->lb_version->setText(VERSION_STRING);
    ui->lb_builddate->setText(BUILD_DATE_STRING);
}

About *About::singleObj = NULL;
About* About::getInstance()
{
    if (singleObj == NULL) {
        singleObj = new About;
    }

    return singleObj;
}

About::~About()
{
    delete ui;
    delete singleObj;
}
