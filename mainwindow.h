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
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <about.h>

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
    void on_comboBox_currentIndexChanged_internal(const QString currentText, QComboBox *cb);
    void updateReadDataUi(QRadioButton* btn, QByteArray readData, int *cnt, QTextEdit* edit, QLabel* label);
    void initIntervalSpinBox(QSpinBox* spinBox);
    void initPortUi(QPushButton* btnOpen,
                    QSpinBox* spinBox,
                    QCheckBox* chkBox,
                    QPushButton* btnSend,
                    QLabel *cntRecvLb,
                    int cntRecv,
                    QLabel *cntSendLb,
                    int cntSend,
                    QRadioButton* asciiSend,
                    QRadioButton* asciiRecv);
    void updateUiOpened(QComboBox *comboBox0,
                        QPushButton* btnOpen,
                        QPushButton* btnSend,
                        QComboBox* comboBox1, QComboBox *comboBox2, QComboBox *comboBox3, QComboBox *comboBox4, QComboBox *comboBox5,
                        QSpinBox* spinBox,
                        QCheckBox* chkBox
                        );

    void updateUiClosed(QComboBox *comboBox0,
                        QPushButton* btnOpen,
                        QPushButton* btnSend,
                        QSpinBox* spinBox,
                        QCheckBox* chkBox,
                        QComboBox* comboBox1, QComboBox *comboBox2, QComboBox *comboBox3, QComboBox *comboBox4, QComboBox *comboBox5, QPushButton *btnSendClr, QTextEdit *edit);
    void limitCharsInTextEdit(const QTextEdit* textEdit, const int maxChars);
    Ui::MainWindow* ui;
    QStringList localConfigData;
    QSerialPort* port[PORT_NUM];
    int sendCnt[8];
    int recvCnt[8];
    MyWorker* worker[PORT_NUM];
    QThread* thread[PORT_NUM];
    QTimer* timer[PORT_NUM];
    bool openFlag[PORT_NUM];
    MainWindow *sub;

signals:
    void sigOpen1(QString portName);


    void sigSend1(QString str, bool isHex, int sendCnt);


    void sigClose1();


    void sigContinueSend1(QString str, int interval, int sendCnt, bool isHex);


private slots:

    void updateSendCntUi1();


    void storeReadData1(QByteArray readData);


    void updateReadDataUi1();


    void updateSendCnt1(int cnt);


    void exitThread1();


    void updateUiOpened1();


    void cannotOpenNotify1();


    void configPort();
    void recvConfigData(QStringList configData);
    void closeEvent(QCloseEvent*);

    // COM7
    void on_btn_send_1_clicked();


    void on_btn_open_1_clicked();

    void updateVisibleArea1();


    void on_checkBox_12_clicked();


    void on_btn_clr_11_clicked();
    void on_btn_clr_12_clicked();


    void on_actionAbout_triggered();
    void on_actionPreferences_triggered();

    void on_actionNewWindow_triggered();

    void on_comboBox_11_currentIndexChanged(const QString &arg1);


    void on_comboBox_16_activated(const QString &arg1);

    void on_comboBox_16_editTextChanged(const QString &arg1);

    void on_comboBox_16_currentTextChanged(const QString &arg1);

    void on_comboBox_16_highlighted(const QString &arg1);

    void on_radio_ascii_12_clicked();

    void on_radio_hex_12_clicked();

private:
    QMenu *menu[10];
    QAction *action[10];
    About *about;

};

#endif // MAINWINDOW_H
