#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include "ser.h"
#include <QSerialPort>
#include <QEventLoop>
#include <QTimer>
#include <QMessageBox>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>

int configured_flag = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    for (int i=0; i<PORT_NUM; i++) {

        port[i] = NULL;
        sendCnt[i] = 0;
        recvCnt[i] = 0;
        worker[i] = NULL;
        thread[i] = NULL;
    }

    localConfigData = (QStringList() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0");

    const QStringList lst = (QStringList() << "9600" << "19200" << "38400" << "57600" << "115200");


    // COM1
    ui->spinBox_11->setMinimum(TIME_INTERVAL_MIN);
    ui->spinBox_11->setMaximum(TIME_INTERVAL_MAX);
    ui->spinBox_11->setSingleStep(TIME_INTERVAL_STEP);
    ui->spinBox_11->setValue(TIME_INTERVAL_DEFAULT);
    ui->comboBox_11->addItems(lst);

    ui->btn_open_1->setDisabled(false);
    ui->btn_close_1->setDisabled(true);
    ui->spinBox_11->setDisabled(true);
    ui->checkBox_12->setDisabled(true);
    ui->btn_send_1->setDisabled(true);
    ui->label_cnt_11->setText(QString::number(recvCnt[0]));
    ui->label_cnt_12->setText(QString::number(sendCnt[0]));
    ui->radio_ascii_11->setChecked(true);
    ui->radio_ascii_12->setChecked(true);

    // COM2
    ui->spinBox_21->setMinimum(TIME_INTERVAL_MIN);
    ui->spinBox_21->setMaximum(TIME_INTERVAL_MAX);
    ui->spinBox_21->setSingleStep(TIME_INTERVAL_STEP);
    ui->spinBox_21->setValue(TIME_INTERVAL_DEFAULT);
    ui->comboBox_21->addItems(lst);

    ui->btn_open_2->setDisabled(false);
    ui->btn_close_2->setDisabled(true);
    ui->spinBox_21->setDisabled(true);
    ui->checkBox_22->setDisabled(true);
    ui->btn_send_2->setDisabled(true);
    ui->label_cnt_21->setText(QString::number(recvCnt[1]));
    ui->label_cnt_22->setText(QString::number(sendCnt[1]));
    ui->radio_ascii_21->setChecked(true);
    ui->radio_ascii_22->setChecked(true);

    // COM3
    ui->spinBox_31->setMinimum(TIME_INTERVAL_MIN);
    ui->spinBox_31->setMaximum(TIME_INTERVAL_MAX);
    ui->spinBox_31->setSingleStep(TIME_INTERVAL_STEP);
    ui->spinBox_31->setValue(TIME_INTERVAL_DEFAULT);
    ui->comboBox_31->addItems(lst);

    ui->btn_open_3->setDisabled(false);
    ui->btn_close_3->setDisabled(true);
    ui->spinBox_31->setDisabled(true);
    ui->checkBox_32->setDisabled(true);
    ui->btn_send_3->setDisabled(true);
    ui->label_cnt_31->setText(QString::number(recvCnt[2]));
    ui->label_cnt_32->setText(QString::number(sendCnt[2]));

    // COM4
    ui->spinBox_41->setMinimum(TIME_INTERVAL_MIN);
    ui->spinBox_41->setMaximum(TIME_INTERVAL_MAX);
    ui->spinBox_41->setSingleStep(TIME_INTERVAL_STEP);
    ui->spinBox_41->setValue(TIME_INTERVAL_DEFAULT);
    ui->comboBox_41->addItems(lst);

    ui->btn_open_4->setDisabled(false);
    ui->btn_close_4->setDisabled(true);
    ui->spinBox_41->setDisabled(true);
    ui->checkBox_42->setDisabled(true);
    ui->btn_send_4->setDisabled(true);
    ui->label_cnt_41->setText(QString::number(recvCnt[3]));
    ui->label_cnt_42->setText(QString::number(sendCnt[3]));

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
    ui->label_cnt_51->setText(QString::number(recvCnt[4]));
    ui->label_cnt_52->setText(QString::number(sendCnt[4]));

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
    ui->label_cnt_61->setText(QString::number(recvCnt[5]));
    ui->label_cnt_62->setText(QString::number(sendCnt[5]));

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
    ui->label_cnt_71->setText(QString::number(recvCnt[6]));
    ui->label_cnt_72->setText(QString::number(sendCnt[6]));

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
    ui->label_cnt_81->setText(QString::number(recvCnt[7]));
    ui->label_cnt_82->setText(QString::number(sendCnt[7]));

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

    // COM1
    ui->groupBox_11->setTitle(localConfigData[0]);
    ui->groupBox_12->setTitle(localConfigData[0]);
    ui->comboBox_11->setEnabled(true);

    // COM2
    ui->groupBox_21->setTitle(localConfigData[1]);
    ui->groupBox_22->setTitle(localConfigData[1]);
    ui->comboBox_21->setEnabled(true);

    // COM3
    ui->groupBox_31->setTitle(localConfigData[2]);
    ui->groupBox_32->setTitle(localConfigData[2]);
    ui->comboBox_31->setEnabled(true);

    // COM4
    ui->groupBox_41->setTitle(localConfigData[3]);
    ui->groupBox_42->setTitle(localConfigData[3]);
    ui->comboBox_41->setEnabled(true);

    // COM5
    ui->groupBox_51->setTitle(localConfigData[4]);
    ui->groupBox_52->setTitle(localConfigData[4]);
    ui->comboBox_51->setEnabled(true);

    // COM6
    ui->groupBox_61->setTitle(localConfigData[5]);
    ui->groupBox_62->setTitle(localConfigData[5]);
    ui->comboBox_61->setEnabled(true);

    // COM7
    ui->groupBox_71->setTitle(localConfigData[6]);
    ui->groupBox_72->setTitle(localConfigData[6]);
    ui->comboBox_71->setEnabled(true);

    // COM8
    ui->groupBox_81->setTitle(localConfigData[7]);
    ui->groupBox_82->setTitle(localConfigData[7]);
    ui->comboBox_81->setEnabled(true);


    configured_flag = 1;

    /*
    on_btn_close_1_clicked();
    on_btn_close_2_clicked();
    on_btn_close_3_clicked();
    on_btn_close_4_clicked();
    on_btn_close_5_clicked();
    on_btn_close_6_clicked();
    on_btn_close_7_clicked();
    on_btn_close_8_clicked();
    */
}

void MainWindow::on_btn_send_1_clicked()
{

    QString str = ui->textEdit_12->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    emit sigSend1(str, ui->radio_hex_12->isChecked(), sendCnt[0]);

    ui->label_cnt_12->setText(QString::number(sendCnt[0]));

}

void MainWindow::on_btn_send_2_clicked()
{

    QString str = ui->textEdit_22->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    emit sigSend2(str, ui->radio_hex_22->isChecked(), sendCnt[1]);

    ui->label_cnt_22->setText(QString::number(sendCnt[1]));

}

void MainWindow::on_btn_send_3_clicked()
{

    QString str = ui->textEdit_32->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[2]->write(str.toLocal8Bit());

    sendCnt[2] += str.size();
    ui->label_cnt_32->setText(QString::number(sendCnt[2]));

}

void MainWindow::on_btn_send_4_clicked()
{

    QString str = ui->textEdit_42->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[3]->write(str.toLocal8Bit());

    sendCnt[3] += str.size();
    ui->label_cnt_42->setText(QString::number(sendCnt[3]));

}

void MainWindow::on_btn_send_5_clicked()
{

    QString str = ui->textEdit_52->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[4]->write(str.toLocal8Bit());

    sendCnt[4] += str.size();
    ui->label_cnt_52->setText(QString::number(sendCnt[4]));

}

void MainWindow::on_btn_send_6_clicked()
{
    QString str = ui->textEdit_62->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[5]->write(str.toLocal8Bit());

    sendCnt[5] += str.size();
    ui->label_cnt_62->setText(QString::number(sendCnt[5]));

}

void MainWindow::on_btn_send_7_clicked()
{

    QString str = ui->textEdit_72->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[6]->write(str.toLocal8Bit());

    sendCnt[6] += str.size();
    ui->label_cnt_72->setText(QString::number(sendCnt[6]));

}

void MainWindow::on_btn_send_8_clicked()
{
    QString str = ui->textEdit_82->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    }

    port[7]->write(str.toLocal8Bit());

    sendCnt[7] += str.size();
    ui->label_cnt_82->setText(QString::number(sendCnt[7]));

}

void MainWindow::handleData()
{

    qDebug() << "handleData";

}

void MainWindow::updateUiOpened1()
{
    ui->btn_open_1->setDisabled(true);
    ui->comboBox_11->setDisabled(true);
    ui->btn_close_1->setDisabled(false);
    ui->spinBox_11->setDisabled(false);
    ui->checkBox_12->setDisabled(false);
    ui->btn_send_1->setDisabled(false);
}

void MainWindow::updateUiOpened2()
{
    ui->btn_open_2->setDisabled(true);
    ui->comboBox_21->setDisabled(true);
    ui->btn_close_2->setDisabled(false);
    ui->spinBox_21->setDisabled(false);
    ui->checkBox_22->setDisabled(false);
    ui->btn_send_2->setDisabled(false);
}

void MainWindow::updateSendCntUi1(int cnt)
{
    sendCnt[0] = cnt;
    ui->label_cnt_12->setText(QString::number(sendCnt[0]));
}

void MainWindow::updateSendCntUi2(int cnt)
{
    sendCnt[1] = cnt;
    ui->label_cnt_22->setText(QString::number(sendCnt[1]));
}

void MainWindow::cannotOpenNotify1()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    emit sigClose1();
}

void MainWindow::cannotOpenNotify2()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    emit sigClose2();
}

void MainWindow::on_btn_open_1_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[0];

    worker[0] = new MyWorker();
    worker[0]->setter(portName, baudRateLst[ui->comboBox_11->currentIndex()]);
    thread[0] = new QThread();
    connect(thread[0], SIGNAL(finished()), worker[0], SLOT(deleteLater()));
    connect(worker[0], SIGNAL(dataReady()), this, SLOT(handleData()));
    connect(worker[0], SIGNAL(sigUpdateSendCntUi(int)), this, SLOT(updateSendCntUi1(int)));
    connect(worker[0], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify1()));
    connect(worker[0], SIGNAL(sigExitThread()), this, SLOT(exitThread1()));
    connect(worker[0], SIGNAL(sigOpened()), this, SLOT(updateUiOpened1()));
    connect(worker[0], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(updateReadDataUi1(QByteArray)));
    connect(this, SIGNAL(sigOpen1()), worker[0], SLOT(doOpen()));
    connect(this, SIGNAL(sigSend1(QString, bool, int)), worker[0], SLOT(doSend(QString, bool, int)));
    connect(this, SIGNAL(sigClose1()), worker[0], SLOT(doClose()));
    connect(this, SIGNAL(sigContinueSend1(QString, int, int, bool)), worker[0], SLOT(doContinueSend(QString, int, int, bool)));
    connect(ui->textEdit_11, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea1()));

    worker[0]->moveToThread(thread[0]);
    thread[0]->start();

    emit sigOpen1();

}

void MainWindow::on_btn_open_2_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[1];

    //worker2[1] = new MyWorker2();
    worker[1] = new MyWorker();
    worker[1]->setter(portName, baudRateLst[ui->comboBox_21->currentIndex()]);
    thread[1] = new QThread();
    connect(thread[1], SIGNAL(finished()), worker[1], SLOT(deleteLater()));
    connect(worker[1], SIGNAL(dataReady()), this, SLOT(handleData()));
    connect(worker[1], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(updateReadDataUi2(QByteArray)));
    connect(worker[1], SIGNAL(sigExitThread()), this, SLOT(exitThread2()));
    connect(worker[1], SIGNAL(sigOpened()), this, SLOT(updateUiOpened2()));
    connect(this, SIGNAL(sigOpen2()), worker[1], SLOT(doOpen()));
    connect(this, SIGNAL(sigSend2(QString, bool, int)), worker[1], SLOT(doSend(QString, bool, int)));
    connect(this, SIGNAL(sigClose2()), worker[1], SLOT(doClose()));
    connect(this, SIGNAL(sigContinueSend2(QString, int, int, bool)), worker[1], SLOT(doContinueSend(QString, int, int, bool)));
    connect(ui->textEdit_21, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea2()));
    connect(worker[1], SIGNAL(sigUpdateSendCntUi(int)), this, SLOT(updateSendCntUi2(int)));
    connect(worker[1], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify2()));
    worker[1]->moveToThread(thread[1]);
    thread[1]->start();

    emit sigOpen2();

}

void MainWindow::on_btn_open_3_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[2];

    port[2] = new QSerialPort(portName);
    port[2]->setBaudRate(baudRateLst[ui->comboBox_31->currentIndex()]);
    port[2]->setDataBits(QSerialPort::Data8);
    port[2]->setParity(QSerialPort::NoParity);
    port[2]->setStopBits(QSerialPort::OneStop);
    port[2]->setFlowControl(QSerialPort::NoFlowControl);

    if (port[2]->open(QIODevice::ReadWrite)) {

        ui->btn_open_3->setDisabled(true);
        ui->comboBox_31->setDisabled(true);
        ui->btn_close_3->setDisabled(false);
        ui->spinBox_31->setDisabled(false);
        ui->checkBox_32->setDisabled(false);
        ui->btn_send_3->setDisabled(false);

        connect(port[2], SIGNAL(readyRead()), this, SLOT(readPort3()));
        connect(ui->textEdit_31, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea3()));

    } else {

        QMessageBox::about(NULL, "警告", "打开串口失败");

    }

}

void MainWindow::on_btn_open_4_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[3];

    port[3] = new QSerialPort(portName);
    port[3]->setBaudRate(baudRateLst[ui->comboBox_41->currentIndex()]);
    port[3]->setDataBits(QSerialPort::Data8);
    port[3]->setParity(QSerialPort::NoParity);
    port[3]->setStopBits(QSerialPort::OneStop);
    port[3]->setFlowControl(QSerialPort::NoFlowControl);

    if (port[3]->open(QIODevice::ReadWrite)) {

        ui->btn_open_4->setDisabled(true);
        ui->comboBox_41->setDisabled(true);
        ui->btn_close_4->setDisabled(false);
        ui->spinBox_41->setDisabled(false);
        ui->checkBox_42->setDisabled(false);
        ui->btn_send_4->setDisabled(false);

        connect(port[3], SIGNAL(readyRead()), this, SLOT(readPort4()));
        connect(ui->textEdit_41, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea4()));

    } else {

        QMessageBox::about(NULL, "警告", "打开串口失败");

    }

}

void MainWindow::on_btn_open_5_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[4];

    port[4] = new QSerialPort(portName);
    port[4]->setBaudRate(baudRateLst[ui->comboBox_51->currentIndex()]);
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

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[5];

    port[5] = new QSerialPort(portName);
    port[5]->setBaudRate(baudRateLst[ui->comboBox_61->currentIndex()]);
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

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[7-1];

    port[6] = new QSerialPort(portName);
    port[6]->setBaudRate(baudRateLst[ui->comboBox_71->currentIndex()]);
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

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[7];

    port[7] = new QSerialPort(portName);
    port[7]->setBaudRate(baudRateLst[ui->comboBox_81->currentIndex()]);
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

void MainWindow::exitThread1()
{
    thread[0]->exit();
    thread[0]->wait();
}

void MainWindow::exitThread2()
{
    thread[1]->exit();
    thread[1]->wait();
}

void MainWindow::on_btn_close_1_clicked()
{
    //if (port[0] != NULL && port[0]->isOpen()) {

        ui->checkBox_12->setCheckState(Qt::Unchecked);
        intervalGen(100);

        qDebug() << "mainclose";
        worker[0]->continueFlag = false;
        emit sigClose1();


        ui->btn_open_1->setDisabled(false);
        ui->btn_close_1->setEnabled(false);
        ui->comboBox_11->setDisabled(false);
        ui->spinBox_11->setDisabled(true);
        ui->checkBox_12->setDisabled(true);
        ui->btn_send_1->setDisabled(true);
    //}
}

void MainWindow::on_btn_close_2_clicked()
{
    //if (port[1] != NULL && port[1]->isOpen()) {

        ui->checkBox_22->setCheckState(Qt::Unchecked);
        intervalGen(100);

        qDebug() << "mainclose";
        worker[1]->continueFlag = false;
        emit sigClose2();

        ui->btn_open_2->setDisabled(false);
        ui->btn_close_2->setEnabled(false);
        ui->comboBox_21->setDisabled(false);
        ui->spinBox_21->setDisabled(true);
        ui->checkBox_22->setDisabled(true);
        ui->btn_send_2->setDisabled(true);
    //}
}

void MainWindow::on_btn_close_3_clicked()
{
    if (port[2] != NULL && port[2]->isOpen()) {

        ui->checkBox_32->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[2]->close();

        ui->btn_open_3->setDisabled(false);
        ui->btn_close_3->setEnabled(false);
        ui->comboBox_31->setDisabled(false);
        ui->spinBox_31->setDisabled(true);
        ui->checkBox_32->setDisabled(true);
        ui->btn_send_3->setDisabled(true);
    }
}

void MainWindow::on_btn_close_4_clicked()
{
    if (port[3] != NULL && port[3]->isOpen()) {

        ui->checkBox_42->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[3]->close();

        ui->btn_open_4->setDisabled(false);
        ui->btn_close_4->setEnabled(false);
        ui->comboBox_41->setDisabled(false);
        ui->spinBox_41->setDisabled(true);
        ui->checkBox_42->setDisabled(true);
        ui->btn_send_4->setDisabled(true);
    }
}

void MainWindow::on_btn_close_5_clicked()
{
    if (port[4] != NULL && port[4]->isOpen()) {

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
    if (port[5] != NULL && port[5]->isOpen()) {

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
    if (port[6] != NULL && port[6]->isOpen()) {

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
    if (port[7] != NULL && port[7]->isOpen()) {

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

void MainWindow::on_checkBox_12_clicked()
{

    QString str;

    if (ui->textEdit_12->toPlainText() == NULL) {

        ui->checkBox_12->setCheckState(Qt::Unchecked);
        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");
        return;

    }

    if (ui->checkBox_12->isChecked()) {

        str = ui->textEdit_12->toPlainText();
        emit sigContinueSend1(str, ui->spinBox_11->value(), sendCnt[0], ui->radio_hex_12->isChecked());

        ui->btn_send_1->setDisabled(true);
        ui->textEdit_12->setReadOnly(true);
        ui->spinBox_11->setDisabled(true);
        ui->btn_clr_12->setDisabled(true);
        ui->radio_ascii_11->setDisabled(true);
        ui->radio_ascii_12->setDisabled(true);
        ui->radio_hex_11->setDisabled(true);
        ui->radio_hex_12->setDisabled(true);

    } else {

        worker[0]->continueFlag = false;
        ui->btn_send_1->setEnabled(true);
        ui->textEdit_12->setReadOnly(false);
        ui->spinBox_11->setDisabled(false);
        ui->btn_clr_12->setDisabled(false);
        ui->radio_ascii_11->setDisabled(false);
        ui->radio_ascii_12->setDisabled(false);
        ui->radio_hex_11->setDisabled(false);
        ui->radio_hex_12->setDisabled(false);
    }

}

void MainWindow::on_checkBox_22_clicked()
{

    QString str;

    if (ui->textEdit_22->toPlainText() == NULL) {

        ui->checkBox_22->setCheckState(Qt::Unchecked);
        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");
        return;

    }

    if (ui->checkBox_22->isChecked()) {

        str = ui->textEdit_22->toPlainText();
        emit sigContinueSend2(str, ui->spinBox_21->value(), sendCnt[1], ui->radio_hex_22->isChecked());

        ui->btn_send_2->setDisabled(true);
        ui->textEdit_22->setReadOnly(true);
        ui->spinBox_21->setDisabled(true);
        ui->btn_clr_22->setDisabled(true);
        ui->radio_ascii_21->setDisabled(true);
        ui->radio_ascii_22->setDisabled(true);
        ui->radio_hex_21->setDisabled(true);
        ui->radio_hex_22->setDisabled(true);

    } else {

        worker[1]->continueFlag = false;
        ui->btn_send_2->setEnabled(true);
        ui->textEdit_22->setReadOnly(false);
        ui->spinBox_21->setDisabled(false);
        ui->btn_clr_22->setDisabled(false);
        ui->radio_ascii_21->setDisabled(false);
        ui->radio_ascii_22->setDisabled(false);
        ui->radio_hex_21->setDisabled(false);
        ui->radio_hex_22->setDisabled(false);
    }
}

void MainWindow::on_checkBox_32_clicked()
{

    QString str;

    if (ui->textEdit_32->toPlainText() == NULL) {

        ui->checkBox_32->setCheckState(Qt::Unchecked);
        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");
        return;

    }

    if (ui->checkBox_32->isChecked()) {

        ui->btn_send_3->setDisabled(true);
        ui->textEdit_32->setReadOnly(true);
        ui->spinBox_31->setDisabled(true);
        ui->btn_clr_32->setDisabled(true);

    } else {

        ui->btn_send_3->setEnabled(true);
        ui->textEdit_32->setReadOnly(false);
        ui->spinBox_31->setDisabled(false);
        ui->btn_clr_32->setDisabled(false);
    }

    for (;;) {

        if (ui->checkBox_32->isChecked() && port[2]->isOpen()) {

            str = ui->textEdit_32->toPlainText();

            port[2]->write(str.toLocal8Bit());

            sendCnt[2] += str.size();
            ui->label_cnt_32->setText(QString::number(sendCnt[2]));

            intervalGen(ui->spinBox_31->value());

        } else {

            ui->textEdit_32->setReadOnly(false);
            ui->btn_clr_32->setDisabled(false);
            break;

        }

    }

}

void MainWindow::on_checkBox_42_clicked()
{

    QString str;

    if (ui->textEdit_42->toPlainText() == NULL) {

        ui->checkBox_42->setCheckState(Qt::Unchecked);
        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");
        return;

    }

    if (ui->checkBox_42->isChecked()) {

        ui->btn_send_4->setDisabled(true);
        ui->textEdit_42->setReadOnly(true);
        ui->spinBox_41->setDisabled(true);
        ui->btn_clr_42->setDisabled(true);

    } else {

        ui->btn_send_4->setEnabled(true);
        ui->textEdit_42->setReadOnly(false);
        ui->spinBox_41->setDisabled(false);
        ui->btn_clr_42->setDisabled(false);
    }

    for (;;) {

        if (ui->checkBox_42->isChecked() && port[3]->isOpen()) {

            str = ui->textEdit_42->toPlainText();

            port[3]->write(str.toLocal8Bit());

            sendCnt[3] += str.size();
            ui->label_cnt_42->setText(QString::number(sendCnt[3]));

            intervalGen(ui->spinBox_41->value());

        } else {

            ui->textEdit_42->setReadOnly(false);
            ui->btn_clr_42->setDisabled(false);
            break;

        }

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
        ui->btn_clr_52->setDisabled(true);

    } else {

        ui->btn_send_5->setEnabled(true);
        ui->textEdit_52->setReadOnly(false);
        ui->spinBox_51->setDisabled(false);
        ui->btn_clr_52->setDisabled(false);
    }

    for (;;) {

        if (ui->checkBox_52->isChecked() && port[4]->isOpen()) {

            str = ui->textEdit_52->toPlainText();

            port[4]->write(str.toLocal8Bit());

            sendCnt[4] += str.size();
            ui->label_cnt_52->setText(QString::number(sendCnt[4]));

            intervalGen(ui->spinBox_51->value());

        } else {

            ui->textEdit_52->setReadOnly(false);
            ui->btn_clr_52->setDisabled(false);
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
        ui->btn_clr_62->setDisabled(true);

    } else {

        ui->btn_send_6->setEnabled(true);
        ui->textEdit_62->setReadOnly(false);
        ui->spinBox_61->setDisabled(false);
        ui->btn_clr_62->setDisabled(false);

    }

    for (;;) {

        if (ui->checkBox_62->isChecked() && port[5]->isOpen()) {

            str = ui->textEdit_62->toPlainText();

            port[5]->write(str.toLocal8Bit());

            sendCnt[5] += str.size();
            ui->label_cnt_62->setText(QString::number(sendCnt[5]));

            intervalGen(ui->spinBox_61->value());

        } else {

            ui->textEdit_62->setReadOnly(false);
            ui->btn_clr_62->setDisabled(false);
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
        ui->btn_clr_72->setDisabled(true);

    } else {

        ui->btn_send_7->setEnabled(true);
        ui->textEdit_72->setReadOnly(false);
        ui->spinBox_71->setDisabled(false);
        ui->btn_clr_72->setDisabled(false);

    }

    for (;;) {

        if (ui->checkBox_72->isChecked() && port[6]->isOpen()) {

            str = ui->textEdit_72->toPlainText();

            port[6]->write(str.toLocal8Bit());

            sendCnt[6] += str.size();
            ui->label_cnt_72->setText(QString::number(sendCnt[6]));

            intervalGen(ui->spinBox_71->value());

        } else {

            ui->textEdit_72->setReadOnly(false);
            ui->btn_clr_72->setDisabled(false);
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
        ui->btn_clr_82->setDisabled(true);

    } else {

        ui->btn_send_8->setEnabled(true);
        ui->textEdit_82->setReadOnly(false);
        ui->spinBox_81->setDisabled(false);
        ui->btn_clr_82->setDisabled(false);

    }

    for (;;) {

        if (ui->checkBox_82->isChecked() && port[7]->isOpen()) {

            str = ui->textEdit_82->toPlainText();

            port[7]->write(str.toLocal8Bit());


            intervalGen(ui->spinBox_81->value());

        } else {

            ui->textEdit_82->setReadOnly(false);
            ui->btn_clr_82->setDisabled(false);
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

void MainWindow::on_btn_clr_11_clicked()
{
    ui->textEdit_11->clear();
    recvCnt[0] = 0;
    ui->label_cnt_11->setText(QString::number(recvCnt[0]));
}

void MainWindow::on_btn_clr_12_clicked()
{
    ui->textEdit_12->clear();
    sendCnt[0] = 0;
    ui->label_cnt_12->setText(QString::number(sendCnt[0]));
}

void MainWindow::on_btn_clr_21_clicked()
{
    ui->textEdit_21->clear();
    recvCnt[1] = 0;
    ui->label_cnt_21->setText(QString::number(recvCnt[1]));
}

void MainWindow::on_btn_clr_22_clicked()
{
    ui->textEdit_22->clear();
    sendCnt[1] = 0;
    ui->label_cnt_22->setText(QString::number(sendCnt[1]));
}

void MainWindow::on_btn_clr_31_clicked()
{
    ui->textEdit_31->clear();
    recvCnt[2] = 0;
    ui->label_cnt_31->setText(QString::number(recvCnt[2]));
}

void MainWindow::on_btn_clr_32_clicked()
{
    ui->textEdit_32->clear();
    sendCnt[2] = 0;
    ui->label_cnt_32->setText(QString::number(sendCnt[2]));
}

void MainWindow::on_btn_clr_41_clicked()
{
    ui->textEdit_41->clear();
    recvCnt[3] = 0;
    ui->label_cnt_41->setText(QString::number(recvCnt[3]));
}

void MainWindow::on_btn_clr_42_clicked()
{
    ui->textEdit_42->clear();
    sendCnt[3] = 0;
    ui->label_cnt_42->setText(QString::number(sendCnt[3]));
}

void MainWindow::on_btn_clr_51_clicked()
{
    ui->textEdit_51->clear();
    recvCnt[4] = 0;
    ui->label_cnt_51->setText(QString::number(recvCnt[4]));
}

void MainWindow::on_btn_clr_52_clicked()
{
    ui->textEdit_52->clear();
    sendCnt[4] = 0;
    ui->label_cnt_52->setText(QString::number(sendCnt[4]));
}

void MainWindow::on_btn_clr_61_clicked()
{
    ui->textEdit_61->clear();
    recvCnt[5] = 0;
    ui->label_cnt_61->setText(QString::number(recvCnt[5]));
}

void MainWindow::on_btn_clr_62_clicked()
{
    ui->textEdit_62->clear();
    sendCnt[5] = 0;
    ui->label_cnt_62->setText(QString::number(sendCnt[5]));
}

void MainWindow::on_btn_clr_71_clicked()
{
    ui->textEdit_71->clear();
    recvCnt[6] = 0;
    ui->label_cnt_71->setText(QString::number(recvCnt[6]));
}

void MainWindow::on_btn_clr_72_clicked()
{
    ui->textEdit_72->clear();
    sendCnt[6] = 0;
    ui->label_cnt_72->setText(QString::number(sendCnt[6]));
}

void MainWindow::on_btn_clr_81_clicked()
{
    ui->textEdit_81->clear();
    recvCnt[7] = 0;
    ui->label_cnt_81->setText(QString::number(recvCnt[7]));
}

void MainWindow::on_btn_clr_82_clicked()
{
    ui->textEdit_82->clear();
    sendCnt[7] = 0;
    ui->label_cnt_82->setText(QString::number(sendCnt[7]));
}

void MainWindow::updateVisibleArea1()
{
    ui->textEdit_11->setTextCursor(ui->textEdit_11->textCursor());
}

void MainWindow::updateVisibleArea2()
{
    ui->textEdit_21->setTextCursor(ui->textEdit_21->textCursor());
}

void MainWindow::updateVisibleArea3()
{
    ui->textEdit_31->setTextCursor(ui->textEdit_31->textCursor());
}

void MainWindow::updateVisibleArea4()
{
    ui->textEdit_41->setTextCursor(ui->textEdit_41->textCursor());
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

    // COM1
    if (worker[0] != NULL && thread[0] != NULL) {

        worker[0]->continueFlag = false;
        emit sigClose1();
    }

    // COM2
    if (worker[1] != NULL && thread[1] != NULL) {

        worker[1]->continueFlag = false;
        emit sigClose2();
    }

    // COM3
    if (port[2] != NULL && port[2]->isOpen()) {

        ui->checkBox_32->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[2]->close();

    }

    // COM4
    if (port[3] != NULL && port[3]->isOpen()) {

        ui->checkBox_42->setCheckState(Qt::Unchecked);
        intervalGen(100);

        port[3]->close();

    }

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

void MainWindow::updateReadDataUi1(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_11, readData, &recvCnt[0], ui->textEdit_11, ui->label_cnt_11);
}

void MainWindow::updateReadDataUi2(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_21, readData, &recvCnt[1], ui->textEdit_21, ui->label_cnt_21);
}

void MainWindow::updateReadDataUi(QRadioButton* btn, QByteArray readData, int* cnt, QTextEdit* edit, QLabel* label)
{
    if (btn->isChecked()) {

        int i = 0;

        readData = readData.toHex();

        while (i < readData.size()) {

            readData.insert(i, " ");
            i += 3;
            *cnt++;

        }

    } else {

        *cnt += readData.count();

    }

    edit->insertPlainText(readData);
    label->setText(QString::number(*cnt));
}
