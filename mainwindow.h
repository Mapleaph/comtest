#ifndef MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "myworker.h"
#include "myworker2.h"
#include <QThread>

#define PORT_NUM 8

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void intervalGen(int interval);
    Ui::MainWindow* ui;
    QStringList localConfigData;
    QSerialPort* port[PORT_NUM];
    int sendCnt[8];
    int recvCnt[8];
    MyWorker* worker[PORT_NUM];
    MyWorker2* worker2[PORT_NUM];
    QThread* thread[PORT_NUM];

signals:
    void sigOpen1();
    void sigOpen2();
    void sigSend1(QString str);
    void sigSend2(QString str);
    void sigClose1();
    void sigClose2();
    void sigContinueSend1(QString str, int interval, int sendCnt);
    void sigContinueSend2(QString str, int interval, int sendCnt);
    void sigStopSend();

private slots:

    void updateReadDataUi1(QByteArray readData);
    void updateReadDataUi2(QByteArray readData);
    void updateSendCntUi1(int cnt);
    void updateSendCntUi2(int cnt);
    void exitThread1();
    void exitThread2();
    void updateUiOpened1();
    void updateUiOpened2();
    void on_btn_config_clicked();
    void cannotOpenNotify1();
    void cannotOpenNotify2();

    void recvConfigData(QStringList configData);

    void closeEvent(QCloseEvent*);

    void handleData();
    // COM7
    void on_btn_send_7_clicked();

    void on_btn_open_7_clicked();

    void on_btn_close_7_clicked();

    void readPort7();

    void updateVisibleArea7();

    void on_checkBox_72_clicked();

    void on_btn_clr_71_clicked();

    void on_btn_clr_72_clicked();

    // COM8
    void on_btn_clr_82_clicked();

    void on_btn_clr_81_clicked();

    void on_btn_open_8_clicked();

    void on_btn_close_8_clicked();

    void on_checkBox_82_clicked();

    void on_btn_send_8_clicked();

    void updateVisibleArea8();

    void readPort8();

    // COM5
    void on_btn_clr_52_clicked();

    void on_btn_clr_51_clicked();

    void on_btn_open_5_clicked();

    void on_btn_close_5_clicked();

    void on_checkBox_52_clicked();

    void on_btn_send_5_clicked();

    void updateVisibleArea5();

    void readPort5();

    // COM6
    void on_btn_clr_62_clicked();

    void on_btn_clr_61_clicked();

    void on_btn_open_6_clicked();

    void on_btn_close_6_clicked();

    void on_checkBox_62_clicked();

    void on_btn_send_6_clicked();

    void updateVisibleArea6();

    void readPort6();

    // COM4
    void on_btn_clr_42_clicked();

    void on_btn_clr_41_clicked();

    void on_btn_open_4_clicked();

    void on_btn_close_4_clicked();

    void on_checkBox_42_clicked();

    void on_btn_send_4_clicked();

    void updateVisibleArea4();

    void readPort4();

    // COM3
    void on_btn_clr_32_clicked();

    void on_btn_clr_31_clicked();

    void on_btn_open_3_clicked();

    void on_btn_close_3_clicked();

    void on_checkBox_32_clicked();

    void on_btn_send_3_clicked();

    void updateVisibleArea3();

    void readPort3();

    // COM2
    void on_btn_clr_22_clicked();

    void on_btn_clr_21_clicked();

    void on_btn_open_2_clicked();

    void on_btn_close_2_clicked();

    void on_checkBox_22_clicked();

    void on_btn_send_2_clicked();

    void updateVisibleArea2();

    void readPort2();

    // COM1
    void on_btn_clr_12_clicked();

    void on_btn_clr_11_clicked();

    void on_btn_open_1_clicked();

    void on_btn_close_1_clicked();

    void on_checkBox_12_clicked();

    void on_btn_send_1_clicked();

    void updateVisibleArea1();

    void readPort1();

private:


};

#endif // MAINWINDOW_H
