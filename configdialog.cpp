#include "configdialog.h"
#include "ui_configdialog.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{

    ui->setupUi(this);

    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();

    if (serialPortInfoList.size() == 0) {

        ui->btn_auto_config->setDisabled(true);
        ui->btn_ok->setDisabled(true);
        ui->comboBox11->setDisabled(true);
        ui->comboBox21->setDisabled(true);
        ui->comboBox31->setDisabled(true);
        ui->comboBox41->setDisabled(true);
        ui->comboBox51->setDisabled(true);
        ui->comboBox61->setDisabled(true);
        ui->comboBox71->setDisabled(true);
        ui->comboBox81->setDisabled(true);
        ui->btn_save_config->setDisabled(true);

    } else {

        foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {

            ui->comboBox11->addItem(serialPortInfo.portName());
            ui->comboBox21->addItem(serialPortInfo.portName());
            ui->comboBox31->addItem(serialPortInfo.portName());
            ui->comboBox41->addItem(serialPortInfo.portName());
            ui->comboBox51->addItem(serialPortInfo.portName());
            ui->comboBox61->addItem(serialPortInfo.portName());
            ui->comboBox71->addItem(serialPortInfo.portName());
            ui->comboBox81->addItem(serialPortInfo.portName());

        }
    }

}

ConfigDialog::~ConfigDialog()
{
    delete ui;
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

    emit sendConfigData(configData);

    QDialog::accept();

}

void ConfigDialog::on_btn_auto_config_clicked()
{

    QString configFilePath = QCoreApplication::applicationDirPath() + "/config.ini";
    QFile configFile(configFilePath);

    int idx[8];
    int i=0;

    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        while (!configFile.atEnd()) {

            QByteArray barr = configFile.readLine().trimmed();

            QString str(barr);

            idx[i] = ui->comboBox11->findText(str);

            if (idx[i] == -1) {

                QMessageBox::about(NULL, "警告", "配置文件读取错误");
                goto exit;

            }

            i++;
        }

        if (i != 8) {

            goto exit;

        }

        ui->comboBox11->setCurrentIndex(idx[0]);
        ui->comboBox21->setCurrentIndex(idx[1]);
        ui->comboBox31->setCurrentIndex(idx[2]);
        ui->comboBox41->setCurrentIndex(idx[3]);
        ui->comboBox51->setCurrentIndex(idx[4]);
        ui->comboBox61->setCurrentIndex(idx[5]);
        ui->comboBox71->setCurrentIndex(idx[6]);
        ui->comboBox81->setCurrentIndex(idx[7]);

        QMessageBox::about(NULL, "通知", "配置文件读取成功");



    } else {

        QMessageBox::about(NULL, "警告", "配置文件读取错误");

    }

exit:
    configFile.close();

}

void ConfigDialog::on_btn_save_config_clicked()
{
    QString configFilePath = QCoreApplication::applicationDirPath() + "/config.ini";
    QFile configFile(configFilePath);

    if (configFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {

        configFile.write(ui->comboBox11->currentText().toLatin1() + "\n");
        configFile.write(ui->comboBox21->currentText().toLatin1() + "\n");
        configFile.write(ui->comboBox31->currentText().toLatin1() + "\n");
        configFile.write(ui->comboBox41->currentText().toLatin1() + "\n");
        configFile.write(ui->comboBox51->currentText().toLatin1() + "\n");
        configFile.write(ui->comboBox61->currentText().toLatin1() + "\n");
        configFile.write(ui->comboBox71->currentText().toLatin1() + "\n");
        configFile.write(ui->comboBox81->currentText().toLatin1());

        configFile.close();

        QMessageBox::about(NULL, "通知", "配置文件写入成功");

    } else {

        QMessageBox::about(NULL, "警告", "配置文件写入失败");

    }
}
