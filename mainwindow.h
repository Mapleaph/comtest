#ifndef MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "myworker.h"
#include <QThread>
#include <QRadioButton>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

#define PORT_NUM 8
#define MAXCHARS 10000

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
    void updateReadDataUi(QRadioButton* btn, QByteArray readData, int *cnt, QTextEdit* edit, QLabel* label);
    void initIntervalSpinBox(QSpinBox* spinBox);
    void initPortUi(QPushButton* btnOpen,
                    QPushButton* btnClose,
                    QSpinBox* spinBox,
                    QCheckBox* chkBox,
                    QPushButton* btnSend,
                    QLabel *cntRecvLb,
                    int cntRecv,
                    QLabel *cntSendLb,
                    int cntSend,
                    QRadioButton* asciiSend,
                    QRadioButton* asciiRecv);
    void updateUiOpened(QPushButton* btnOpen,
                        QPushButton* btnClose,
                        QPushButton* btnSend,
                        QComboBox* comboBox1, QComboBox *comboBox2, QComboBox *comboBox3, QComboBox *comboBox4, QComboBox *comboBox5,
                        QSpinBox* spinBox,
                        QCheckBox* chkBox
                        );

    void updateUiClosed(QPushButton* btnOpen,
                        QPushButton* btnClose,
                        QPushButton* btnSend,
                        QSpinBox* spinBox,
                        QCheckBox* chkBox,
                        QComboBox* comboBox1, QComboBox *comboBox2, QComboBox *comboBox3, QComboBox *comboBox4, QComboBox *comboBox5, QPushButton *btnSendClr, QTextEdit *edit);
    void limitCharsInTextEdit(const QTextEdit* textEdit, const int maxChars);
    bool configCheck(DataBitsType dataBits, StopBitsType stopBits);
    Ui::MainWindow* ui;
    QStringList localConfigData;
    QSerialPort* port[PORT_NUM];
    int sendCnt[8];
    int recvCnt[8];
    MyWorker* worker[PORT_NUM];
    QThread* thread[PORT_NUM];
    QTimer* timer[PORT_NUM];
    bool openFlag[PORT_NUM];

signals:
    void sigOpen1();
    void sigOpen2();
    void sigOpen3();
    void sigOpen4();
    void sigOpen5();
    void sigOpen6();
    void sigOpen7();
    void sigOpen8();

    void sigSend1(QString str, bool isHex, int sendCnt);
    void sigSend2(QString str, bool isHex, int sendCnt);
    void sigSend3(QString str, bool isHex, int sendCnt);
    void sigSend4(QString str, bool isHex, int sendCnt);
    void sigSend5(QString str, bool isHex, int sendCnt);
    void sigSend6(QString str, bool isHex, int sendCnt);
    void sigSend7(QString str, bool isHex, int sendCnt);
    void sigSend8(QString str, bool isHex, int sendCnt);

    void sigClose1();
    void sigClose2();
    void sigClose3();
    void sigClose4();
    void sigClose5();
    void sigClose6();
    void sigClose7();
    void sigClose8();

    void sigContinueSend1(QString str, int interval, int sendCnt, bool isHex);
    void sigContinueSend2(QString str, int interval, int sendCnt, bool isHex);
    void sigContinueSend3(QString str, int interval, int sendCnt, bool isHex);
    void sigContinueSend4(QString str, int interval, int sendCnt, bool isHex);
    void sigContinueSend5(QString str, int interval, int sendCnt, bool isHex);
    void sigContinueSend6(QString str, int interval, int sendCnt, bool isHex);
    void sigContinueSend7(QString str, int interval, int sendCnt, bool isHex);
    void sigContinueSend8(QString str, int interval, int sendCnt, bool isHex);

private slots:

    void updateSendCntUi1();
    void updateSendCntUi2();
    void updateSendCntUi3();
    void updateSendCntUi4();
    void updateSendCntUi5();
    void updateSendCntUi6();
    void updateSendCntUi7();
    void updateSendCntUi8();

    void storeReadData1(QByteArray readData);
    void storeReadData2(QByteArray readData);
    void storeReadData3(QByteArray readData);
    void storeReadData4(QByteArray readData);
    void storeReadData5(QByteArray readData);
    void storeReadData6(QByteArray readData);
    void storeReadData7(QByteArray readData);
    void storeReadData8(QByteArray readData);

    void updateReadDataUi1();
    void updateReadDataUi2();
    void updateReadDataUi3();
    void updateReadDataUi4();
    void updateReadDataUi5();
    void updateReadDataUi6();
    void updateReadDataUi7();
    void updateReadDataUi8();

    void updateSendCnt1(int cnt);
    void updateSendCnt2(int cnt);
    void updateSendCnt3(int cnt);
    void updateSendCnt4(int cnt);
    void updateSendCnt5(int cnt);
    void updateSendCnt6(int cnt);
    void updateSendCnt7(int cnt);
    void updateSendCnt8(int cnt);

    void exitThread1();
    void exitThread2();
    void exitThread3();
    void exitThread4();
    void exitThread5();
    void exitThread6();
    void exitThread7();
    void exitThread8();

    void updateUiOpened1();
    void updateUiOpened2();
    void updateUiOpened3();
    void updateUiOpened4();
    void updateUiOpened5();
    void updateUiOpened6();
    void updateUiOpened7();
    void updateUiOpened8();

    void cannotOpenNotify1();
    void cannotOpenNotify2();
    void cannotOpenNotify3();
    void cannotOpenNotify4();
    void cannotOpenNotify5();
    void cannotOpenNotify6();
    void cannotOpenNotify7();
    void cannotOpenNotify8();

    void on_btn_config_clicked();
    void recvConfigData(QStringList configData);
    void closeEvent(QCloseEvent*);

    // COM7
    void on_btn_send_1_clicked();
    void on_btn_send_2_clicked();
    void on_btn_send_3_clicked();
    void on_btn_send_4_clicked();
    void on_btn_send_5_clicked();
    void on_btn_send_6_clicked();
    void on_btn_send_7_clicked();
    void on_btn_send_8_clicked();

    void on_btn_open_1_clicked();
    void on_btn_open_2_clicked();
    void on_btn_open_3_clicked();
    void on_btn_open_4_clicked();
    void on_btn_open_5_clicked();
    void on_btn_open_6_clicked();
    void on_btn_open_7_clicked();
    void on_btn_open_8_clicked();

    void on_btn_close_1_clicked();
    void on_btn_close_2_clicked();
    void on_btn_close_3_clicked();
    void on_btn_close_4_clicked();
    void on_btn_close_5_clicked();
    void on_btn_close_6_clicked();
    void on_btn_close_7_clicked();
    void on_btn_close_8_clicked();

    void updateVisibleArea1();
    void updateVisibleArea2();
    void updateVisibleArea3();
    void updateVisibleArea4();
    void updateVisibleArea5();
    void updateVisibleArea6();
    void updateVisibleArea7();
    void updateVisibleArea8();

    void on_checkBox_12_clicked();
    void on_checkBox_22_clicked();
    void on_checkBox_32_clicked();
    void on_checkBox_42_clicked();
    void on_checkBox_52_clicked();
    void on_checkBox_62_clicked();
    void on_checkBox_72_clicked();
    void on_checkBox_82_clicked();

    void on_btn_clr_11_clicked();
    void on_btn_clr_12_clicked();
    void on_btn_clr_21_clicked();
    void on_btn_clr_22_clicked();
    void on_btn_clr_31_clicked();
    void on_btn_clr_32_clicked();
    void on_btn_clr_41_clicked();
    void on_btn_clr_42_clicked();
    void on_btn_clr_51_clicked();
    void on_btn_clr_52_clicked();
    void on_btn_clr_61_clicked();
    void on_btn_clr_62_clicked();
    void on_btn_clr_71_clicked();
    void on_btn_clr_72_clicked();
    void on_btn_clr_81_clicked();
    void on_btn_clr_82_clicked();

private:


};

#endif // MAINWINDOW_H
