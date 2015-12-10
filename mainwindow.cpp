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

void MainWindow::initIntervalSpinBox(QSpinBox* spinBox)
{
    spinBox->setMinimum(TIME_INTERVAL_MIN);
    spinBox->setMaximum(TIME_INTERVAL_MAX);
    spinBox->setSingleStep(TIME_INTERVAL_STEP);
    spinBox->setValue(TIME_INTERVAL_DEFAULT);
}

void MainWindow::initPortUi(QPushButton* btnOpen,
                            QPushButton* btnClose,
                            QSpinBox* spinBox,
                            QCheckBox* chkBox,
                            QPushButton* btnSend,
                            QLabel* cntRecvLb,
                            int cntRecv,
                            QLabel* cntSendLb,
                            int cntSend,
                            QRadioButton* asciiSend,
                            QRadioButton* asciiRecv)
{
    btnOpen->setDisabled(false);
    btnClose->setDisabled(true);
    spinBox->setDisabled(true);
    chkBox->setDisabled(true);
    btnSend->setDisabled(true);
    cntSendLb->setText(QString::number(cntSend));
    cntRecvLb->setText(QString::number(cntRecv));
    asciiRecv->setChecked(true);
    asciiSend->setChecked(true);
}

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
    initIntervalSpinBox(ui->spinBox_11);
    ui->comboBox_11->addItems(lst);
    initPortUi(ui->btn_open_1,
               ui->btn_close_1,
               ui->spinBox_11,
               ui->checkBox_12,
               ui->btn_send_1,
               ui->label_cnt_11,
               recvCnt[0],
            ui->label_cnt_12,
            sendCnt[0],
            ui->radio_ascii_11,
            ui->radio_ascii_12);

    // COM2
    initIntervalSpinBox(ui->spinBox_21);
    ui->comboBox_21->addItems(lst);

    initPortUi(ui->btn_open_2,
               ui->btn_close_2,
               ui->spinBox_21,
               ui->checkBox_22,
               ui->btn_send_2,
               ui->label_cnt_21,
               recvCnt[1],
            ui->label_cnt_22,
            sendCnt[1],
            ui->radio_ascii_21,
            ui->radio_ascii_22);

    // COM3
    initIntervalSpinBox(ui->spinBox_31);
    ui->comboBox_31->addItems(lst);

    initPortUi(ui->btn_open_3,
               ui->btn_close_3,
               ui->spinBox_31,
               ui->checkBox_32,
               ui->btn_send_3,
               ui->label_cnt_31,
               recvCnt[2],
            ui->label_cnt_32,
            sendCnt[2],
            ui->radio_ascii_31,
            ui->radio_ascii_32);

    // COM4
    initIntervalSpinBox(ui->spinBox_41);
    ui->comboBox_41->addItems(lst);

    initPortUi(ui->btn_open_4,
               ui->btn_close_4,
               ui->spinBox_41,
               ui->checkBox_42,
               ui->btn_send_4,
               ui->label_cnt_41,
               recvCnt[3],
            ui->label_cnt_42,
            sendCnt[3],
            ui->radio_ascii_41,
            ui->radio_ascii_42);

    // COM5
    initIntervalSpinBox(ui->spinBox_51);
    ui->comboBox_51->addItems(lst);

    initPortUi(ui->btn_open_5,
               ui->btn_close_5,
               ui->spinBox_51,
               ui->checkBox_52,
               ui->btn_send_5,
               ui->label_cnt_51,
               recvCnt[4],
            ui->label_cnt_52,
            sendCnt[4],
            ui->radio_ascii_51,
            ui->radio_ascii_52);

    // COM6
    initIntervalSpinBox(ui->spinBox_61);
    ui->comboBox_61->addItems(lst);

    initPortUi(ui->btn_open_6,
               ui->btn_close_6,
               ui->spinBox_61,
               ui->checkBox_62,
               ui->btn_send_6,
               ui->label_cnt_61,
               recvCnt[5],
            ui->label_cnt_62,
            sendCnt[5],
            ui->radio_ascii_61,
            ui->radio_ascii_62);

    // COM7
    initIntervalSpinBox(ui->spinBox_71);
    ui->comboBox_71->addItems(lst);

    initPortUi(ui->btn_open_7,
               ui->btn_close_7,
               ui->spinBox_71,
               ui->checkBox_72,
               ui->btn_send_7,
               ui->label_cnt_71,
               recvCnt[6],
            ui->label_cnt_72,
            sendCnt[6],
            ui->radio_ascii_71,
            ui->radio_ascii_72);

    // COM8
    initIntervalSpinBox(ui->spinBox_81);
    ui->comboBox_81->addItems(lst);

    initPortUi(ui->btn_open_8,
               ui->btn_close_8,
               ui->spinBox_81,
               ui->checkBox_82,
               ui->btn_send_8,
               ui->label_cnt_81,
               recvCnt[7],
            ui->label_cnt_82,
            sendCnt[7],
            ui->radio_ascii_81,
            ui->radio_ascii_82);

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

//    on_btn_close_1_clicked();
//    on_btn_close_2_clicked();
//    on_btn_close_3_clicked();
//    on_btn_close_4_clicked();
//    on_btn_close_5_clicked();
//    on_btn_close_6_clicked();
//    on_btn_close_7_clicked();
//    on_btn_close_8_clicked();

}

// section send

void MainWindow::on_btn_send_1_clicked()
{

    QString str = ui->textEdit_12->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    } else {

        emit sigSend1(str, ui->radio_hex_12->isChecked(), sendCnt[0]);

        ui->label_cnt_12->setText(QString::number(sendCnt[0]));

    }

}

void MainWindow::on_btn_send_2_clicked()
{

    QString str = ui->textEdit_22->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    } else {

        emit sigSend2(str, ui->radio_hex_22->isChecked(), sendCnt[1]);

        ui->label_cnt_22->setText(QString::number(sendCnt[1]));

    }

}

void MainWindow::on_btn_send_3_clicked()
{

    QString str = ui->textEdit_32->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    } else {

        emit sigSend3(str, ui->radio_hex_32->isChecked(), sendCnt[2]);

        ui->label_cnt_32->setText(QString::number(sendCnt[2]));

    }

}

void MainWindow::on_btn_send_4_clicked()
{

    QString str = ui->textEdit_42->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    } else {

        emit sigSend4(str, ui->radio_hex_42->isChecked(), sendCnt[3]);

        ui->label_cnt_42->setText(QString::number(sendCnt[3]));

    }

}

void MainWindow::on_btn_send_5_clicked()
{

    QString str = ui->textEdit_52->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    } else {

        emit sigSend5(str, ui->radio_hex_52->isChecked(), sendCnt[4]);

        ui->label_cnt_52->setText(QString::number(sendCnt[4]));

    }

}

void MainWindow::on_btn_send_6_clicked()
{

    QString str = ui->textEdit_62->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    } else {

        emit sigSend6(str, ui->radio_hex_62->isChecked(), sendCnt[5]);

        ui->label_cnt_62->setText(QString::number(sendCnt[5]));

    }

}

void MainWindow::on_btn_send_7_clicked()
{

    QString str = ui->textEdit_72->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    } else {

        emit sigSend7(str, ui->radio_hex_72->isChecked(), sendCnt[6]);

        ui->label_cnt_72->setText(QString::number(sendCnt[6]));

    }

}

void MainWindow::on_btn_send_8_clicked()
{

    QString str = ui->textEdit_82->toPlainText();

    if (str == NULL) {

        QMessageBox::about(NULL, "警告", "请先输入要发送的数据");

    } else {

        emit sigSend8(str, ui->radio_hex_82->isChecked(), sendCnt[7]);

        ui->label_cnt_82->setText(QString::number(sendCnt[7]));

    }

}

// section send end

// section updateUiOpened

void MainWindow::updateUiOpened(QPushButton* btnOpen,
                    QPushButton* btnClose,
                    QPushButton* btnSend,
                    QComboBox* comboBox,
                    QSpinBox* spinBox,
                    QCheckBox* chkBox
                    )
{
    btnOpen->setDisabled(true);
    btnClose->setDisabled(false);
    btnSend->setDisabled(false);
    comboBox->setDisabled(true);
    spinBox->setDisabled(false);
    chkBox->setDisabled(false);
}

void MainWindow::updateUiOpened1()
{
    updateUiOpened(ui->btn_open_1,
                   ui->btn_close_1,
                   ui->btn_send_1,
                   ui->comboBox_11,
                   ui->spinBox_11,
                   ui->checkBox_12);
}

void MainWindow::updateUiOpened2()
{
    updateUiOpened(ui->btn_open_2,
                   ui->btn_close_2,
                   ui->btn_send_2,
                   ui->comboBox_21,
                   ui->spinBox_21,
                   ui->checkBox_22);
}

void MainWindow::updateUiOpened3()
{
    updateUiOpened(ui->btn_open_3,
                   ui->btn_close_3,
                   ui->btn_send_3,
                   ui->comboBox_31,
                   ui->spinBox_31,
                   ui->checkBox_32);
}

void MainWindow::updateUiOpened4()
{
    updateUiOpened(ui->btn_open_4,
                   ui->btn_close_4,
                   ui->btn_send_4,
                   ui->comboBox_41,
                   ui->spinBox_41,
                   ui->checkBox_42);
}

void MainWindow::updateUiOpened5()
{
    updateUiOpened(ui->btn_open_5,
                   ui->btn_close_5,
                   ui->btn_send_5,
                   ui->comboBox_51,
                   ui->spinBox_51,
                   ui->checkBox_52);
}

void MainWindow::updateUiOpened6()
{
    updateUiOpened(ui->btn_open_6,
                   ui->btn_close_6,
                   ui->btn_send_6,
                   ui->comboBox_61,
                   ui->spinBox_61,
                   ui->checkBox_62);
}

void MainWindow::updateUiOpened7()
{
    updateUiOpened(ui->btn_open_7,
                   ui->btn_close_7,
                   ui->btn_send_7,
                   ui->comboBox_71,
                   ui->spinBox_71,
                   ui->checkBox_72);
}

void MainWindow::updateUiOpened8()
{
    updateUiOpened(ui->btn_open_8,
                   ui->btn_close_8,
                   ui->btn_send_8,
                   ui->comboBox_81,
                   ui->spinBox_81,
                   ui->checkBox_82);
}

// section updateUiOpened end

// section updateSendCntUi

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

void MainWindow::updateSendCntUi3(int cnt)
{
    sendCnt[2] = cnt;
    ui->label_cnt_32->setText(QString::number(sendCnt[2]));
}

void MainWindow::updateSendCntUi4(int cnt)
{
    sendCnt[3] = cnt;
    ui->label_cnt_42->setText(QString::number(sendCnt[3]));
}

void MainWindow::updateSendCntUi5(int cnt)
{
    sendCnt[4] = cnt;
    ui->label_cnt_52->setText(QString::number(sendCnt[4]));
}

void MainWindow::updateSendCntUi6(int cnt)
{
    sendCnt[5] = cnt;
    ui->label_cnt_62->setText(QString::number(sendCnt[5]));
}

void MainWindow::updateSendCntUi7(int cnt)
{
    sendCnt[6] = cnt;
    ui->label_cnt_72->setText(QString::number(sendCnt[6]));
}

void MainWindow::updateSendCntUi8(int cnt)
{
    sendCnt[7] = cnt;
    ui->label_cnt_82->setText(QString::number(sendCnt[7]));
}

// section updateSendCntUi end

// section cannotOpenNotify

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

void MainWindow::cannotOpenNotify3()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    emit sigClose3();
}

void MainWindow::cannotOpenNotify4()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    emit sigClose4();
}

void MainWindow::cannotOpenNotify5()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    emit sigClose5();
}

void MainWindow::cannotOpenNotify6()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    emit sigClose6();
}

void MainWindow::cannotOpenNotify7()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    emit sigClose7();
}

void MainWindow::cannotOpenNotify8()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    emit sigClose8();
}

// section cannotOpenNotify end

// section btn_open

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

    worker[2] = new MyWorker();
    worker[2]->setter(portName, baudRateLst[ui->comboBox_31->currentIndex()]);
    thread[2] = new QThread();
    connect(thread[2], SIGNAL(finished()), worker[2], SLOT(deleteLater()));
    connect(worker[2], SIGNAL(sigUpdateSendCntUi(int)), this, SLOT(updateSendCntUi3(int)));
    connect(worker[2], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify3()));
    connect(worker[2], SIGNAL(sigExitThread()), this, SLOT(exitThread3()));
    connect(worker[2], SIGNAL(sigOpened()), this, SLOT(updateUiOpened3()));
    connect(worker[2], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(updateReadDataUi3(QByteArray)));
    connect(this, SIGNAL(sigOpen3()), worker[2], SLOT(doOpen()));
    connect(this, SIGNAL(sigSend3(QString, bool, int)), worker[2], SLOT(doSend(QString, bool, int)));
    connect(this, SIGNAL(sigClose3()), worker[2], SLOT(doClose()));
    connect(this, SIGNAL(sigContinueSend3(QString, int, int, bool)), worker[2], SLOT(doContinueSend(QString, int, int, bool)));
    connect(ui->textEdit_31, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea3()));

    worker[2]->moveToThread(thread[2]);
    thread[2]->start();

    emit sigOpen3();

}

void MainWindow::on_btn_open_4_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[3];

    worker[3] = new MyWorker();
    worker[3]->setter(portName, baudRateLst[ui->comboBox_41->currentIndex()]);
    thread[3] = new QThread();
    connect(thread[3], SIGNAL(finished()), worker[3], SLOT(deleteLater()));
    connect(worker[3], SIGNAL(sigUpdateSendCntUi(int)), this, SLOT(updateSendCntUi4(int)));
    connect(worker[3], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify4()));
    connect(worker[3], SIGNAL(sigExitThread()), this, SLOT(exitThread4()));
    connect(worker[3], SIGNAL(sigOpened()), this, SLOT(updateUiOpened4()));
    connect(worker[3], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(updateReadDataUi4(QByteArray)));
    connect(this, SIGNAL(sigOpen4()), worker[3], SLOT(doOpen()));
    connect(this, SIGNAL(sigSend4(QString, bool, int)), worker[3], SLOT(doSend(QString, bool, int)));
    connect(this, SIGNAL(sigClose4()), worker[3], SLOT(doClose()));
    connect(this, SIGNAL(sigContinueSend4(QString, int, int, bool)), worker[3], SLOT(doContinueSend(QString, int, int, bool)));
    connect(ui->textEdit_41, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea4()));

    worker[3]->moveToThread(thread[3]);
    thread[3]->start();

    emit sigOpen4();

}

void MainWindow::on_btn_open_5_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[4];

    worker[4] = new MyWorker();
    worker[4]->setter(portName, baudRateLst[ui->comboBox_51->currentIndex()]);
    thread[4] = new QThread();

    connect(thread[4], SIGNAL(finished()), worker[4], SLOT(deleteLater()));
    connect(worker[4], SIGNAL(sigUpdateSendCntUi(int)), this, SLOT(updateSendCntUi5(int)));
    connect(worker[4], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify5()));
    connect(worker[4], SIGNAL(sigExitThread()), this, SLOT(exitThread5()));
    connect(worker[4], SIGNAL(sigOpened()), this, SLOT(updateUiOpened5()));
    connect(worker[4], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(updateReadDataUi5(QByteArray)));

    connect(this, SIGNAL(sigOpen5()), worker[4], SLOT(doOpen()));
    connect(this, SIGNAL(sigSend5(QString, bool, int)), worker[4], SLOT(doSend(QString, bool, int)));
    connect(this, SIGNAL(sigClose5()), worker[4], SLOT(doClose()));
    connect(this, SIGNAL(sigContinueSend5(QString, int, int, bool)), worker[4], SLOT(doContinueSend(QString, int, int, bool)));
    connect(ui->textEdit_51, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea5()));

    worker[4]->moveToThread(thread[4]);
    thread[4]->start();

    emit sigOpen5();

}

void MainWindow::on_btn_open_6_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[5];

    worker[5] = new MyWorker();
    worker[5]->setter(portName, baudRateLst[ui->comboBox_61->currentIndex()]);
    thread[5] = new QThread();

    connect(thread[5], SIGNAL(finished()), worker[5], SLOT(deleteLater()));
    connect(worker[5], SIGNAL(sigUpdateSendCntUi(int)), this, SLOT(updateSendCntUi6(int)));
    connect(worker[5], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify6()));
    connect(worker[5], SIGNAL(sigExitThread()), this, SLOT(exitThread6()));
    connect(worker[5], SIGNAL(sigOpened()), this, SLOT(updateUiOpened6()));
    connect(worker[5], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(updateReadDataUi6(QByteArray)));

    connect(this, SIGNAL(sigOpen6()), worker[5], SLOT(doOpen()));
    connect(this, SIGNAL(sigSend6(QString, bool, int)), worker[5], SLOT(doSend(QString, bool, int)));
    connect(this, SIGNAL(sigClose6()), worker[5], SLOT(doClose()));
    connect(this, SIGNAL(sigContinueSend6(QString, int, int, bool)), worker[5], SLOT(doContinueSend(QString, int, int, bool)));
    connect(ui->textEdit_61, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea6()));

    worker[5]->moveToThread(thread[5]);
    thread[5]->start();

    emit sigOpen6();

}

void MainWindow::on_btn_open_7_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[6];

    worker[6] = new MyWorker();
    worker[6]->setter(portName, baudRateLst[ui->comboBox_71->currentIndex()]);
    thread[6] = new QThread();

    connect(thread[6], SIGNAL(finished()), worker[6], SLOT(deleteLater()));
    connect(worker[6], SIGNAL(sigUpdateSendCntUi(int)), this, SLOT(updateSendCntUi7(int)));
    connect(worker[6], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify7()));
    connect(worker[6], SIGNAL(sigExitThread()), this, SLOT(exitThread7()));
    connect(worker[6], SIGNAL(sigOpened()), this, SLOT(updateUiOpened7()));
    connect(worker[6], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(updateReadDataUi7(QByteArray)));

    connect(this, SIGNAL(sigOpen7()), worker[6], SLOT(doOpen()));
    connect(this, SIGNAL(sigSend7(QString, bool, int)), worker[6], SLOT(doSend(QString, bool, int)));
    connect(this, SIGNAL(sigClose7()), worker[6], SLOT(doClose()));
    connect(this, SIGNAL(sigContinueSend7(QString, int, int, bool)), worker[6], SLOT(doContinueSend(QString, int, int, bool)));
    connect(ui->textEdit_71, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea7()));

    worker[6]->moveToThread(thread[6]);
    thread[6]->start();

    emit sigOpen7();

}

void MainWindow::on_btn_open_8_clicked()
{

    if (configured_flag == 0) {

        QMessageBox::about(NULL, "警告", "请先配置串口");
        return;

    }

    QString portName = localConfigData[7];

    worker[7] = new MyWorker();
    worker[7]->setter(portName, baudRateLst[ui->comboBox_81->currentIndex()]);
    thread[7] = new QThread();

    connect(thread[7], SIGNAL(finished()), worker[7], SLOT(deleteLater()));
    connect(worker[7], SIGNAL(sigUpdateSendCntUi(int)), this, SLOT(updateSendCntUi8(int)));
    connect(worker[7], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify8()));
    connect(worker[7], SIGNAL(sigExitThread()), this, SLOT(exitThread8()));
    connect(worker[7], SIGNAL(sigOpened()), this, SLOT(updateUiOpened8()));
    connect(worker[7], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(updateReadDataUi8(QByteArray)));

    connect(this, SIGNAL(sigOpen8()), worker[7], SLOT(doOpen()));
    connect(this, SIGNAL(sigSend8(QString, bool, int)), worker[7], SLOT(doSend(QString, bool, int)));
    connect(this, SIGNAL(sigClose8()), worker[7], SLOT(doClose()));
    connect(this, SIGNAL(sigContinueSend8(QString, int, int, bool)), worker[7], SLOT(doContinueSend(QString, int, int, bool)));
    connect(ui->textEdit_81, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea8()));

    worker[7]->moveToThread(thread[7]);
    thread[7]->start();

    emit sigOpen8();

}

// section btn_open end

// section exitThread

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

void MainWindow::exitThread3()
{
    thread[2]->exit();
    thread[2]->wait();
}

void MainWindow::exitThread4()
{
    thread[3]->exit();
    thread[3]->wait();
}

void MainWindow::exitThread5()
{
    thread[4]->exit();
    thread[4]->wait();
}

void MainWindow::exitThread6()
{
    thread[5]->exit();
    thread[5]->wait();
}

void MainWindow::exitThread7()
{
    thread[6]->exit();
    thread[6]->wait();
}

void MainWindow::exitThread8()
{
    thread[7]->exit();
    thread[7]->wait();
}

// section exitThread end

// section btn_close
void MainWindow::updateUiClosed(QPushButton* btnOpen,
                                QPushButton* btnClose,
                                QPushButton* btnSend,
                                QSpinBox* spinBox,
                                QCheckBox* chkBox,
                                QComboBox* comboBox)
{

    btnOpen->setDisabled(false);
    btnClose->setEnabled(false);
    btnSend->setDisabled(true);
    spinBox->setDisabled(true);
    chkBox->setDisabled(true);
    comboBox->setDisabled(false);

}

void MainWindow::on_btn_close_1_clicked()
{

    ui->checkBox_12->setCheckState(Qt::Unchecked);

    intervalGen(100);

    worker[0]->continueFlag = false;
    emit sigClose1();

    updateUiClosed(ui->btn_open_1,
                   ui->btn_close_1,
                   ui->btn_send_1,
                   ui->spinBox_11,
                   ui->checkBox_12,
                   ui->comboBox_11);

}

void MainWindow::on_btn_close_2_clicked()
{
    ui->checkBox_22->setCheckState(Qt::Unchecked);

    intervalGen(100);

    worker[1]->continueFlag = false;
    emit sigClose2();

    updateUiClosed(ui->btn_open_2,
                   ui->btn_close_2,
                   ui->btn_send_2,
                   ui->spinBox_21,
                   ui->checkBox_22,
                   ui->comboBox_21);
}

void MainWindow::on_btn_close_3_clicked()
{
    ui->checkBox_32->setCheckState(Qt::Unchecked);

    intervalGen(100);

    worker[2]->continueFlag = false;
    emit sigClose3();

    updateUiClosed(ui->btn_open_3,
                   ui->btn_close_3,
                   ui->btn_send_3,
                   ui->spinBox_31,
                   ui->checkBox_32,
                   ui->comboBox_31);
}

void MainWindow::on_btn_close_4_clicked()
{
    ui->checkBox_42->setCheckState(Qt::Unchecked);

    intervalGen(100);

    worker[3]->continueFlag = false;
    emit sigClose4();

    updateUiClosed(ui->btn_open_4,
                   ui->btn_close_4,
                   ui->btn_send_4,
                   ui->spinBox_41,
                   ui->checkBox_42,
                   ui->comboBox_41);
}

void MainWindow::on_btn_close_5_clicked()
{
    ui->checkBox_52->setCheckState(Qt::Unchecked);

    intervalGen(100);

    worker[4]->continueFlag = false;
    emit sigClose5();

    updateUiClosed(ui->btn_open_5,
                   ui->btn_close_5,
                   ui->btn_send_5,
                   ui->spinBox_51,
                   ui->checkBox_52,
                   ui->comboBox_51);
}

void MainWindow::on_btn_close_6_clicked()
{
    ui->checkBox_62->setCheckState(Qt::Unchecked);

    intervalGen(100);

    worker[5]->continueFlag = false;
    emit sigClose6();

    updateUiClosed(ui->btn_open_6,
                   ui->btn_close_6,
                   ui->btn_send_6,
                   ui->spinBox_61,
                   ui->checkBox_62,
                   ui->comboBox_61);
}

void MainWindow::on_btn_close_7_clicked()
{
    ui->checkBox_72->setCheckState(Qt::Unchecked);

    intervalGen(100);

    worker[6]->continueFlag = false;
    emit sigClose7();

    updateUiClosed(ui->btn_open_7,
                   ui->btn_close_7,
                   ui->btn_send_7,
                   ui->spinBox_71,
                   ui->checkBox_72,
                   ui->comboBox_71);
}

void MainWindow::on_btn_close_8_clicked()
{
    ui->checkBox_82->setCheckState(Qt::Unchecked);

    intervalGen(100);

    worker[7]->continueFlag = false;
    emit sigClose8();

    updateUiClosed(ui->btn_open_8,
                   ui->btn_close_8,
                   ui->btn_send_8,
                   ui->spinBox_81,
                   ui->checkBox_82,
                   ui->comboBox_81);
}

// section btn_close end

// section checkBox

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
        emit sigContinueSend1(str, ui->spinBox_11->value(),
                              sendCnt[0], ui->radio_hex_12->isChecked());

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

        str = ui->textEdit_32->toPlainText();
        emit sigContinueSend3(str, ui->spinBox_31->value(),
                              sendCnt[2], ui->radio_hex_32->isChecked());

        ui->btn_send_3->setDisabled(true);
        ui->textEdit_32->setReadOnly(true);
        ui->spinBox_31->setDisabled(true);
        ui->btn_clr_32->setDisabled(true);
        ui->radio_ascii_31->setDisabled(true);
        ui->radio_ascii_32->setDisabled(true);
        ui->radio_hex_31->setDisabled(true);
        ui->radio_hex_32->setDisabled(true);

    } else {

        worker[2]->continueFlag = false;
        ui->btn_send_3->setEnabled(true);
        ui->textEdit_32->setReadOnly(false);
        ui->spinBox_31->setDisabled(false);
        ui->btn_clr_32->setDisabled(false);
        ui->radio_ascii_31->setDisabled(false);
        ui->radio_ascii_32->setDisabled(false);
        ui->radio_hex_31->setDisabled(false);
        ui->radio_hex_32->setDisabled(false);
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

        str = ui->textEdit_42->toPlainText();
        emit sigContinueSend4(str, ui->spinBox_41->value(),
                              sendCnt[3], ui->radio_hex_42->isChecked());

        ui->btn_send_4->setDisabled(true);
        ui->textEdit_42->setReadOnly(true);
        ui->spinBox_41->setDisabled(true);
        ui->btn_clr_42->setDisabled(true);
        ui->radio_ascii_41->setDisabled(true);
        ui->radio_ascii_42->setDisabled(true);
        ui->radio_hex_41->setDisabled(true);
        ui->radio_hex_42->setDisabled(true);

    } else {

        worker[3]->continueFlag = false;
        ui->btn_send_4->setEnabled(true);
        ui->textEdit_42->setReadOnly(false);
        ui->spinBox_41->setDisabled(false);
        ui->btn_clr_42->setDisabled(false);
        ui->radio_ascii_41->setDisabled(false);
        ui->radio_ascii_42->setDisabled(false);
        ui->radio_hex_41->setDisabled(false);
        ui->radio_hex_42->setDisabled(false);
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

        str = ui->textEdit_52->toPlainText();
        emit sigContinueSend5(str, ui->spinBox_51->value(),
                              sendCnt[4], ui->radio_hex_52->isChecked());

        ui->btn_send_5->setDisabled(true);
        ui->textEdit_52->setReadOnly(true);
        ui->spinBox_51->setDisabled(true);
        ui->btn_clr_52->setDisabled(true);
        ui->radio_ascii_51->setDisabled(true);
        ui->radio_ascii_52->setDisabled(true);
        ui->radio_hex_51->setDisabled(true);
        ui->radio_hex_52->setDisabled(true);

    } else {

        worker[4]->continueFlag = false;
        ui->btn_send_5->setEnabled(true);
        ui->textEdit_52->setReadOnly(false);
        ui->spinBox_51->setDisabled(false);
        ui->btn_clr_52->setDisabled(false);
        ui->radio_ascii_51->setDisabled(false);
        ui->radio_ascii_52->setDisabled(false);
        ui->radio_hex_51->setDisabled(false);
        ui->radio_hex_52->setDisabled(false);
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

        str = ui->textEdit_62->toPlainText();
        emit sigContinueSend6(str, ui->spinBox_61->value(),
                              sendCnt[5], ui->radio_hex_62->isChecked());

        ui->btn_send_6->setDisabled(true);
        ui->textEdit_62->setReadOnly(true);
        ui->spinBox_61->setDisabled(true);
        ui->btn_clr_62->setDisabled(true);
        ui->radio_ascii_61->setDisabled(true);
        ui->radio_ascii_62->setDisabled(true);
        ui->radio_hex_61->setDisabled(true);
        ui->radio_hex_62->setDisabled(true);

    } else {

        worker[5]->continueFlag = false;
        ui->btn_send_6->setEnabled(true);
        ui->textEdit_62->setReadOnly(false);
        ui->spinBox_61->setDisabled(false);
        ui->btn_clr_62->setDisabled(false);
        ui->radio_ascii_61->setDisabled(false);
        ui->radio_ascii_62->setDisabled(false);
        ui->radio_hex_61->setDisabled(false);
        ui->radio_hex_62->setDisabled(false);

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

        str = ui->textEdit_72->toPlainText();
        emit sigContinueSend7(str, ui->spinBox_71->value(),
                              sendCnt[6], ui->radio_hex_72->isChecked());

        ui->btn_send_7->setDisabled(true);
        ui->textEdit_72->setReadOnly(true);
        ui->spinBox_71->setDisabled(true);
        ui->btn_clr_72->setDisabled(true);
        ui->radio_ascii_71->setDisabled(true);
        ui->radio_ascii_72->setDisabled(true);
        ui->radio_hex_71->setDisabled(true);
        ui->radio_hex_72->setDisabled(true);

    } else {

        worker[6]->continueFlag = false;
        ui->btn_send_7->setEnabled(true);
        ui->textEdit_72->setReadOnly(false);
        ui->spinBox_71->setDisabled(false);
        ui->btn_clr_72->setDisabled(false);
        ui->radio_ascii_71->setDisabled(false);
        ui->radio_ascii_72->setDisabled(false);
        ui->radio_hex_71->setDisabled(false);
        ui->radio_hex_72->setDisabled(false);
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

        str = ui->textEdit_82->toPlainText();
        emit sigContinueSend8(str, ui->spinBox_81->value(),
                              sendCnt[7], ui->radio_hex_82->isChecked());

        ui->btn_send_8->setDisabled(true);
        ui->textEdit_82->setReadOnly(true);
        ui->spinBox_81->setDisabled(true);
        ui->btn_clr_82->setDisabled(true);
        ui->radio_ascii_81->setDisabled(true);
        ui->radio_ascii_82->setDisabled(true);
        ui->radio_hex_81->setDisabled(true);
        ui->radio_hex_82->setDisabled(true);

    } else {

        worker[7]->continueFlag = false;
        ui->btn_send_8->setEnabled(true);
        ui->textEdit_82->setReadOnly(false);
        ui->spinBox_81->setDisabled(false);
        ui->btn_clr_82->setDisabled(false);
        ui->radio_ascii_81->setDisabled(false);
        ui->radio_ascii_82->setDisabled(false);
        ui->radio_hex_81->setDisabled(false);
        ui->radio_hex_82->setDisabled(false);
    }

}

// section checkBox end

void MainWindow::intervalGen(int interval)
{
    QEventLoop loop;
    QTimer::singleShot(interval, &loop, SLOT(quit()));
    loop.exec();
}

// section btn_clr

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

// section btn_clr end

// section updateVisibleArea

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

// section updateVisibleArea end

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
    if (worker[2] != NULL && thread[2] != NULL) {

        worker[2]->continueFlag = false;
        emit sigClose3();
    }

    // COM4
    if (worker[3] != NULL && thread[3] != NULL) {

        worker[3]->continueFlag = false;
        emit sigClose4();
    }

    // COM5
    if (worker[4] != NULL && thread[4] != NULL) {

        worker[4]->continueFlag = false;
        emit sigClose5();
    }

    // COM6
    if (worker[5] != NULL && thread[5] != NULL) {

        worker[5]->continueFlag = false;
        emit sigClose6();
    }

    // COM7
    if (worker[6] != NULL && thread[6] != NULL) {

        worker[6]->continueFlag = false;
        emit sigClose7();
    }

    // COM8
    if (worker[7] != NULL && thread[7] != NULL) {

        worker[7]->continueFlag = false;
        emit sigClose8();
    }

}

// section updateReadDataUi

void MainWindow::updateReadDataUi1(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_11, readData, &recvCnt[0], ui->textEdit_11, ui->label_cnt_11);
}

void MainWindow::updateReadDataUi2(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_21, readData, &recvCnt[1], ui->textEdit_21, ui->label_cnt_21);
}

void MainWindow::updateReadDataUi3(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_31, readData, &recvCnt[2], ui->textEdit_31, ui->label_cnt_31);
}

void MainWindow::updateReadDataUi4(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_41, readData, &recvCnt[3], ui->textEdit_41, ui->label_cnt_41);
}

void MainWindow::updateReadDataUi5(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_51, readData, &recvCnt[4], ui->textEdit_51, ui->label_cnt_51);
}

void MainWindow::updateReadDataUi6(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_61, readData, &recvCnt[5], ui->textEdit_61, ui->label_cnt_61);
}

void MainWindow::updateReadDataUi7(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_71, readData, &recvCnt[6], ui->textEdit_71, ui->label_cnt_71);
}

void MainWindow::updateReadDataUi8(QByteArray readData)
{
    updateReadDataUi(ui->radio_hex_81, readData, &recvCnt[7], ui->textEdit_81, ui->label_cnt_81);
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

// sectio updateReadDataUi end
