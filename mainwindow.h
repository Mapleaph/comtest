#ifndef MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

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
    QStringList localConfigData = (QStringList() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0"
                                    << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0");
    QSerialPort* port[PORT_NUM];

private slots:
    void on_btn_config_clicked();

    void recvConfigData(QStringList configData);

    void closeEvent(QCloseEvent*);

    // COM7
    void on_btn_send_7_clicked();

    void on_btn_open_7_clicked();

    void on_btn_close_7_clicked();

    void readPort7();

    void updateVisibleArea7();

    void on_checkBox_72_clicked();

    void on_btn_clr_71_clicked();

    void on_btn_clr_72_clicked();

    void on_comboBox_71_currentIndexChanged(int index);

    // COM8
    void on_btn_clr_82_clicked();

    void on_btn_clr_81_clicked();

    void on_btn_open_8_clicked();

    void on_btn_close_8_clicked();

    void on_comboBox_81_currentIndexChanged(int index);

    void on_checkBox_82_clicked();

    void on_btn_send_8_clicked();

    void updateVisibleArea8();

    void readPort8();

    // COM5
    void on_btn_clr_52_clicked();

    void on_btn_clr_51_clicked();

    void on_btn_open_5_clicked();

    void on_btn_close_5_clicked();

    void on_comboBox_51_currentIndexChanged(int index);

    void on_checkBox_52_clicked();

    void on_btn_send_5_clicked();

    void updateVisibleArea5();

    void readPort5();

    // COM6
    void on_btn_clr_62_clicked();

    void on_btn_clr_61_clicked();

    void on_btn_open_6_clicked();

    void on_btn_close_6_clicked();

    void on_comboBox_61_currentIndexChanged(int index);

    void on_checkBox_62_clicked();

    void on_btn_send_6_clicked();

    void updateVisibleArea6();

    void readPort6();

private:


};

#endif // MAINWINDOW_H
