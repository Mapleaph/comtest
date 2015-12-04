#include "configdialog.h"
#include "ui_configdialog.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{

    ui->setupUi(this);

    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();

    foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {

//        qDebug() << "port " << serialPortInfo.portName();
//        qDebug() << "location " << serialPortInfo.systemLocation();
//        qDebug() << "description" << serialPortInfo.description();
//        qDebug() << "Manufacturer " << serialPortInfo.manufacturer();

        // only add serial ports belongs to our card
        //if (serialPortInfo.vendorIdentifier() == 0x00) {

            ui->comboBox11->addItem(serialPortInfo.portName());
            ui->comboBox21->addItem(serialPortInfo.portName());
            ui->comboBox31->addItem(serialPortInfo.portName());
            ui->comboBox41->addItem(serialPortInfo.portName());
            ui->comboBox51->addItem(serialPortInfo.portName());
            ui->comboBox61->addItem(serialPortInfo.portName());
            ui->comboBox71->addItem(serialPortInfo.portName());
            ui->comboBox81->addItem(serialPortInfo.portName());

        //}
    }


    InitBaudrateComboBox(ui->comboBox12);
    InitBaudrateComboBox(ui->comboBox22);
    InitBaudrateComboBox(ui->comboBox32);
    InitBaudrateComboBox(ui->comboBox42);
    InitBaudrateComboBox(ui->comboBox52);
    InitBaudrateComboBox(ui->comboBox62);
    InitBaudrateComboBox(ui->comboBox72);
    InitBaudrateComboBox(ui->comboBox82);

}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::InitBaudrateComboBox(QComboBox* box)
{
    box->addItem("9600");
    box->addItem("19200");
    box->addItem("38400");
    box->addItem("57600");
    box->addItem("115200");
}

void ConfigDialog::on_btn_ok_clicked()
{
    QStringList configData;

    configData << ui->comboBox11->currentText();
    configData << ui->comboBox21->currentText();
    configData << ui->comboBox31->currentText();
    configData << ui->comboBox41->currentText();
    configData << ui->comboBox51->currentText();
    configData << ui->comboBox61->currentText();
    configData << ui->comboBox71->currentText();
    configData << ui->comboBox81->currentText();

    configData << QString::number(ui->comboBox12->currentIndex());
    configData << QString::number(ui->comboBox22->currentIndex());
    configData << QString::number(ui->comboBox32->currentIndex());
    configData << QString::number(ui->comboBox42->currentIndex());
    configData << QString::number(ui->comboBox52->currentIndex());
    configData << QString::number(ui->comboBox62->currentIndex());
    configData << QString::number(ui->comboBox72->currentIndex());
    configData << QString::number(ui->comboBox82->currentIndex());

    emit sendConfigData(configData);

    QDialog::accept();

}
