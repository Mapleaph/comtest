#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include "ser.h"
#include <QSerialPort>
#include <QEventLoop>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    for (int i=0; i<PORT_NUM; i++) {

        port[i] = NULL;

    }

    const QStringList lst = (QStringList() << "9600" << "19200" << "38400" << "57600" << "115200");


    // COM5
    ui->spinBox_51->setMinimum(TIME_INTERVAL_MIN);
    ui->spinBox_51->setMaximum(TIME_INTERVAL_MAX);
    ui->spinBox_51->setSingleStep(TIME_INTERVAL_STEP);
    ui->spinBox_51->setValue(TIME_INTERVAL_DEFAULT);
    ui->comboBox_51->addItems(lst);

    ui->btn_open_5->setDisabled(false);
    ui->btn_close_5->setDisabled(true);
    ui->spinBox_51->setDisabled(true);
    ui->checkBox_52->setDisabled(true);
    ui->btn_send_5->setDisabled(true);

    // COM6
    ui->spinBox_61->setMinimum(TIME_INTERVAL_MIN);
    ui->spinBox_61->setMaximum(TIME_INTERVAL_MAX);
    ui->spinBox_61->setSingleStep(TIME_INTERVAL_STEP);
    ui->spinBox_61->setValue(TIME_INTERVAL_DEFAULT);
    ui->comboBox_61->addItems(lst);

    ui->btn_open_6->setDisabled(false);
    ui->btn_close_6->setDisabled(true);
    ui->spinBox_61->setDisabled(true);
    ui->checkBox_62->setDisabled(true);
    ui->btn_send_6->setDisabled(true);

    // COM7
    ui->spinBox_71->setMinimum(TIME_INTERVAL_MIN);
    ui->spinBox_71->setMaximum(TIME_INTERVAL_MAX);
    ui->spinBox_71->setSingleStep(TIME_INTERVAL_STEP);
    ui->spinBox_71->setValue(TIME_INTERVAL_DEFAULT);
    ui->comboBox_71->addItems(lst);

    ui->btn_open_7->setDisabled(false);
    ui->btn_close_7->setDisabled(true);
    ui->spinBox_71->setDisabled(true);
    ui->checkBox_72->setDisabled(true);
    ui->btn_send_7->setDisabled(true);

    // COM8
    ui->spinBox_81->setMinimum(TIME_INTERVAL_MIN);
    ui->spinBox_81->setMaximum(TIME_INTERVAL_MAX);
    ui->spinBox_81->setSingleStep(TIME_INTERVAL_STEP);
    ui->spinBox_81->setValue(TIME_INTERVAL_DEFAULT);
    ui->comboBox_81->addItems(lst);

    ui->btn_open_8->setDisabled(false);
    ui->btn_close_8->setDisabled(true);
    ui->spinBox_81->setDisabled(true);
    ui->checkBox_82->setDisabled(true);
    ui->btn_send_8->setDisabled(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_config_clicked()
{

    ConfigDialog *configDialog = new ConfigDialog(this);
    connect(configDialog, SIGNAL(sendConfigData(QStringList)), this, SLOT(recvConfigData(QStringList)));
    configDialog->exec();

}

void MainWindow::recvConfigData(QStringList configData)
{

    localConfigData = configData;

    // COM5
    ui->groupBox_51->setTitle(localConfigData[4]);
    ui->groupBox_52->setTitle(localConfigData[4]);
    ui->comboBox_51->setCurrentIndex(localConfigData[12].toInt());
    ui->comboBox_51->setEnabled(true);

    // COM6
    ui->groupBox_61->setTitle(localConfigData[5]);
    ui->groupBox_62->setTitle(localConfigData[5]);
    ui->comboBox_61->setCurrentIndex(localConfigData[13].toInt());
    ui->comboBox_61->setEnabled(true);

    // COM7
    ui->groupBox_71->setTitle(localConfigData[6]);
    ui->groupBox_72->setTitle(localConfigData[6]);
    ui->comboBox_71->setCurrentIndex(localConfigData[14].toInt());
    ui->comboBox_71->setEnabled(true);

    // COM8
    ui->groupBox_81->setTitle(localConfigData[7]);
    ui->groupBox_82->setTitle(localConfigData[7]);
    ui->comboBox_81->setCurrentIndex(localConfigData[15].toInt());
    ui->comboBox_81->setEnabled(true);

}

void MainWindow::on_btn_send_5_clicked()
{

    QString str = ui->textEdit_52->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[4]->write(str.toLocal8Bit());

}

void MainWindow::on_btn_send_6_clicked()
{
    QString str = ui->textEdit_62->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[5]->write(str.toLocal8Bit());
}

void MainWindow::on_btn_send_7_clicked()
{

    QString str = ui->textEdit_72->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[6]->write(str.toLocal8Bit());

}

void MainWindow::on_btn_send_8_clicked()
{
    QString str = ui->textEdit_82->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[7]->write(str.toLocal8Bit());
}

void MainWindow::on_btn_open_5_clicked()
{

    if (localConfigData.size() != CONFIG_LIST_SIZE) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[4];

    port[4] = new QSerialPort(portName);
    port[4]->setBaudRate(baudRateLst[localConfigData[12].toInt()]);
    port[4]->setDataBits(QSerialPort::Data8);
    port[4]->setParity(QSerialPort::NoParity);
    port[4]->setStopBits(QSerialPort::OneStop);
    port[4]->setFlowControl(QSerialPort::NoFlowControl);

    if (port[4]->open(QIODevice::ReadWrite)) {

        ui->btn_open_5->setDisabled(true);
        ui->comboBox_51->setDisabled(true);
        ui->btn_close_5->setDisabled(false);
        ui->spinBox_51->setDisabled(false);
        ui->checkBox_52->setDisabled(false);
        ui->btn_send_5->setDisabled(false);

        connect(port[4], SIGNAL(readyRead()), this, SLOT(readPort5()));
        connect(ui->textEdit_51, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea5()));

    } else {

        QMessageBox::about(NULL, "警告", "打开串口失败");

    }

}

void MainWindow::on_btn_open_6_clicked()
{

    if (localConfigData.size() != CONFIG_LIST_SIZE) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[5];

    port[5] = new QSerialPort(portName);
    port[5]->setBaudRate(baudRateLst[localConfigData[13].toInt()]);
    port[5]->setDataBits(QSerialPort::Data8);
    port[5]->setParity(QSerialPort::NoParity);
    port[5]->setStopBits(QSerialPort::OneStop);
    port[5]->setFlowControl(QSerialPort::NoFlowControl);

    if (port[5]->open(QIODevice::ReadWrite)) {

        ui->btn_open_6->setDisabled(true);
        ui->comboBox_61->setDisabled(true);
        ui->btn_close_6->setDisabled(false);
        ui->spinBox_61->setDisabled(false);
        ui->checkBox_62->setDisabled(false);
        ui->btn_send_6->setDisabled(false);

        connect(port[5], SIGNAL(readyRead()), this, SLOT(readPort6()));
        connect(ui->textEdit_61, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea6()));

    } else {

        QMessageBox::about(NULL, "警告", "打开串口失败");

    }

}

void MainWindow::on_btn_open_7_clicked()
{

    if (localConfigData.size() != CONFIG_LIST_SIZE) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[7-1];

    port[6] = new QSerialPort(portName);
    port[6]->setBaudRate(baudRateLst[localConfigData[7+7].toInt()]);
    port[6]->setDataBits(QSerialPort::Data8);
    port[6]->setParity(QSerialPort::NoParity);
    port[6]->setStopBits(QSerialPort::OneStop);
    port[6]->setFlowControl(QSerialPort::NoFlowControl);

    if (port[6]->open(QIODevice::ReadWrite)) {

        ui->btn_open_7->setDisabled(true);
        ui->comboBox_71->setDisabled(true);
        ui->btn_close_7->setDisabled(false);
        ui->spinBox_71->setDisabled(false);
        ui->checkBox_72->setDisabled(false);
        ui->btn_send_7->setDisabled(false);

        connect(port[6], SIGNAL(readyRead()), this, SLOT(readPort7()));
        connect(ui->textEdit_71, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea7()));

    } else {

        QMessageBox::about(NULL, "警告", "打开串口失败");

    }

}

void MainWindow::on_btn_open_8_clicked()
{

    if (localConfigData.size() != CONFIG_LIST_SIZE) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[7];

    port[7] = new QSerialPort(portName);
    port[7]->setBaudRate(baudRateLst[localConfigData[15].toInt()]);
    port[7]->setDataBits(QSerialPort::Data8);
    port[7]->setParity(QSerialPort::NoParity);
    port[7]->setStopBits(QSerialPort::OneStop);
    port[7]->setFlowControl(QSerialPort::NoFlowControl);

    if (port[7]->open(QIODevice::ReadWrite)) {

        ui->btn_open_8->setDisabled(true);
        ui->comboBox_81->setDisabled(true);
        ui->btn_close_8->setDisabled(false);
        ui->spinBox_81->setDisabled(false);
        ui->checkBox_82->setDisabled(false);
        ui->btn_send_8->setDisabled(false);

        connect(port[7], SIGNAL(readyRead()), this, SLOT(readPort8()));
        connect(ui->textEdit_81, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea8()));

    } else {

        QMessageBox::about(NULL, "警告", "打开串口失败");

    }
}

void MainWindow::readPort5()
{

    QByteArray readData = port[4]->readAll();
    ui->textEdit_51->insertPlainText(readData);

}

void MainWindow::readPort6()
{

    QByteArray readData = port[5]->readAll();
    ui->textEdit_61->insertPlainText(readData);

}

void MainWindow::readPort7()
{

    QByteArray readData = port[6]->readAll();
    ui->textEdit_71->insertPlainText(readData);

}

void MainWindow::readPort8()
{

    QByteArray readData = port[7]->readAll();
    ui->textEdit_81->insertPlainText(readData);

}

void MainWindow::on_btn_close_5_clicked()
{
    if (port[4]->isOpen()) {

        ui->checkBox_52->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[4]->close();

        ui->btn_open_5->setDisabled(false);
        ui->btn_close_5->setEnabled(false);
        ui->comboBox_51->setDisabled(false);
        ui->spinBox_51->setDisabled(true);
        ui->checkBox_52->setDisabled(true);
        ui->btn_send_5->setDisabled(true);
    }
}

void MainWindow::on_btn_close_6_clicked()
{
    if (port[5]->isOpen()) {

        ui->checkBox_62->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[5]->close();

        ui->btn_open_6->setDisabled(false);
        ui->btn_close_6->setEnabled(false);
        ui->comboBox_61->setDisabled(false);
        ui->spinBox_61->setDisabled(true);
        ui->checkBox_62->setDisabled(true);
        ui->btn_send_6->setDisabled(true);
    }
}

void MainWindow::on_btn_close_7_clicked()
{
    if (port[6]->isOpen()) {

        ui->checkBox_72->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[6]->close();

        ui->btn_open_7->setDisabled(false);
        ui->btn_close_7->setEnabled(false);
        ui->comboBox_71->setDisabled(false);
        ui->spinBox_71->setDisabled(true);
        ui->checkBox_72->setDisabled(true);
        ui->btn_send_7->setDisabled(true);
    }
}

void MainWindow::on_btn_close_8_clicked()
{
    if (port[7]->isOpen()) {

        ui->checkBox_82->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[7]->close();

        ui->btn_open_8->setDisabled(false);
        ui->btn_close_8->setEnabled(false);
        ui->comboBox_81->setDisabled(false);
        ui->spinBox_81->setDisabled(true);
        ui->checkBox_82->setDisabled(true);
        ui->btn_send_8->setDisabled(true);
    }
}

void MainWindow::on_checkBox_52_clicked()
{

    QString str;

    if (ui->textEdit_52->toPlainText() == NULL) {

        ui->checkBox_52->setCheckState(Qt::Unchecked);
        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");
        return;

    }

    if (ui->checkBox_52->isChecked()) {

        ui->btn_send_5->setDisabled(true);
        ui->textEdit_52->setReadOnly(true);
        ui->spinBox_51->setDisabled(true);

    } else {

        ui->btn_send_5->setEnabled(true);
        ui->textEdit_52->setReadOnly(false);
        ui->spinBox_51->setDisabled(false);

    }

    for (;;) {

        if (ui->checkBox_52->isChecked() && port[4]->isOpen()) {

            str = ui->textEdit_52->toPlainText();

            port[4]->write(str.toLocal8Bit());


            intervalGen(ui->spinBox_51->value());

        } else {

            ui->textEdit_52->setReadOnly(false);
            break;

        }

    }

}

void MainWindow::on_checkBox_62_clicked()
{

    QString str;

    if (ui->textEdit_62->toPlainText() == NULL) {

        ui->checkBox_62->setCheckState(Qt::Unchecked);
        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");
        return;

    }

    if (ui->checkBox_62->isChecked()) {

        ui->btn_send_6->setDisabled(true);
        ui->textEdit_62->setReadOnly(true);
        ui->spinBox_61->setDisabled(true);

    } else {

        ui->btn_send_6->setEnabled(true);
        ui->textEdit_62->setReadOnly(false);
        ui->spinBox_61->setDisabled(false);

    }

    for (;;) {

        if (ui->checkBox_62->isChecked() && port[5]->isOpen()) {

            str = ui->textEdit_62->toPlainText();

            port[5]->write(str.toLocal8Bit());


            intervalGen(ui->spinBox_61->value());

        } else {

            ui->textEdit_62->setReadOnly(false);
            break;

        }

    }

}

void MainWindow::on_checkBox_72_clicked()
{

    QString str;

    if (ui->textEdit_72->toPlainText() == NULL) {

        ui->checkBox_72->setCheckState(Qt::Unchecked);
        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");
        return;

    }

    if (ui->checkBox_72->isChecked()) {

        ui->btn_send_7->setDisabled(true);
        ui->textEdit_72->setReadOnly(true);
        ui->spinBox_71->setDisabled(true);

    } else {

        ui->btn_send_7->setEnabled(true);
        ui->textEdit_72->setReadOnly(false);
        ui->spinBox_71->setDisabled(false);

    }

    for (;;) {

        if (ui->checkBox_72->isChecked() && port[6]->isOpen()) {

            str = ui->textEdit_72->toPlainText();

            port[6]->write(str.toLocal8Bit());


            intervalGen(ui->spinBox_71->value());

        } else {

            ui->textEdit_72->setReadOnly(false);
            break;

        }

    }

}

void MainWindow::on_checkBox_82_clicked()
{

    QString str;

    if (ui->textEdit_82->toPlainText() == NULL) {

        ui->checkBox_82->setCheckState(Qt::Unchecked);
        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");
        return;

    }

    if (ui->checkBox_82->isChecked()) {

        ui->btn_send_8->setDisabled(true);
        ui->textEdit_82->setReadOnly(true);
        ui->spinBox_81->setDisabled(true);

    } else {

        ui->btn_send_8->setEnabled(true);
        ui->textEdit_82->setReadOnly(false);
        ui->spinBox_81->setDisabled(false);

    }

    for (;;) {

        if (ui->checkBox_82->isChecked() && port[7]->isOpen()) {

            str = ui->textEdit_82->toPlainText();

            port[7]->write(str.toLocal8Bit());


            intervalGen(ui->spinBox_81->value());

        } else {

            ui->textEdit_82->setReadOnly(false);
            break;

        }

    }

}

void MainWindow::intervalGen(int interval)
{
    QEventLoop loop;
    QTimer::singleShot(interval, &loop, SLOT(quit()));
    loop.exec();
}

void MainWindow::on_btn_clr_51_clicked()
{
    ui->textEdit_51->clear();
}

void MainWindow::on_btn_clr_52_clicked()
{
    ui->textEdit_52->clear();
}

void MainWindow::on_btn_clr_61_clicked()
{
    ui->textEdit_61->clear();
}

void MainWindow::on_btn_clr_62_clicked()
{
    ui->textEdit_62->clear();
}

void MainWindow::on_btn_clr_71_clicked()
{
    ui->textEdit_71->clear();
}

void MainWindow::on_btn_clr_72_clicked()
{
    ui->textEdit_72->clear();
}

void MainWindow::on_btn_clr_81_clicked()
{
    ui->textEdit_81->clear();
}

void MainWindow::on_btn_clr_82_clicked()
{
    ui->textEdit_82->clear();
}

void MainWindow::updateVisibleArea5()
{
    ui->textEdit_51->setTextCursor(ui->textEdit_51->textCursor());
}

void MainWindow::updateVisibleArea6()
{
    ui->textEdit_61->setTextCursor(ui->textEdit_61->textCursor());
}

void MainWindow::updateVisibleArea7()
{
    ui->textEdit_71->setTextCursor(ui->textEdit_71->textCursor());
}

void MainWindow::updateVisibleArea8()
{
    ui->textEdit_81->setTextCursor(ui->textEdit_81->textCursor());
}

void MainWindow::closeEvent(QCloseEvent*)
{

    // COM5
    if (port[4] != NULL && port[4]->isOpen()) {

        ui->checkBox_52->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[4]->close();

    }

    // COM6
    if (port[5] != NULL && port[5]->isOpen()) {

        ui->checkBox_62->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[5]->close();

    }

    // COM7
    if (port[6] != NULL && port[6]->isOpen()) {

        ui->checkBox_72->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[6]->close();

    }

    // COM8
    if (port[7] != NULL && port[7]->isOpen()) {

        ui->checkBox_82->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[7]->close();

    }
}

void MainWindow::on_comboBox_51_currentIndexChanged(int index)
{
    localConfigData[12] = QString::number(index);
}

void MainWindow::on_comboBox_61_currentIndexChanged(int index)
{
    localConfigData[13] = QString::number(index);
}

void MainWindow::on_comboBox_71_currentIndexChanged(int index)
{
    localConfigData[14] = QString::number(index);
}

void MainWindow::on_comboBox_81_currentIndexChanged(int index)
{
    localConfigData[15] = QString::number(index);
}
