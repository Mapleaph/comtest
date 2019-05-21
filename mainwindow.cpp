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
#include <QMenuBar>
#include <QMenu>
#include <QLocale>
#include <QtCore>
#include <QSerialPortInfo>



int configured_flag = 0;
QByteArray tmparr[8];

QFile *dataFile;

void MainWindow::initIntervalSpinBox(QSpinBox* spinBox)
{
    spinBox->setMinimum(TIME_INTERVAL_MIN);
    spinBox->setMaximum(TIME_INTERVAL_MAX);
    spinBox->setSingleStep(TIME_INTERVAL_STEP);
    spinBox->setValue(TIME_INTERVAL_DEFAULT);
}

void MainWindow::initPortUi(QPushButton* btnOpen,
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

    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();

    if (serialPortInfoList.size() == 0) {

        ui->comboBox_10->setDisabled(true);

    } else {

        foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {

            ui->comboBox_10->addItem(serialPortInfo.portName());

        }
    }

    if (QSysInfo::productType() == "windows") {

        //menu[0] = new QMenu(tr("About"));
        action[0] = new QAction(tr("About"), this);

        //menu[0]->addAction(action[0]);
        //ui->menuBar->addMenu(menu[0]);
        ui->menuBar->addAction(action[0]);

        //connect(action[1], SIGNAL(triggered(bool)), this, SLOT(on_actionAbout_triggered()));
        connect(action[0], SIGNAL(triggered(bool)), this, SLOT(on_actionAbout_triggered()));

        ui->label_14->setText(tr("us"));

    } else if (QSysInfo::productType() == "osx") {

        menu[0] = new QMenu(tr("ComTest"));
        action[0] = new QAction(tr("About"), this);
        action[1] = new QAction(tr("Preferences"), this);

        menu[0]->addAction(action[0]);
        menu[0]->addAction(action[1]);
        ui->menuBar->addMenu(menu[0]);

        connect(action[0], SIGNAL(triggered(bool)), this, SLOT(on_actionAbout_triggered()));
        connect(action[1], SIGNAL(triggered(bool)), this, SLOT(on_actionPreferences_triggered()));
    }


//    ui->menuBar->addMenu(menu[1]);
//    ui->menuBar->addMenu(menu[2]);

    //menubar->setNativeMenuBar(true);
    //menubar->setVisible(true);
    //menubar->show();

    QIcon icon(":/images/icon.icns");
    setWindowIcon(icon);

    //for (int i=0; i<PORT_NUM; i++) {
    for (int i=0; i<1; i++) {

        port[i] = NULL;
        sendCnt[i] = 0;
        recvCnt[i] = 0;
        worker[i] = NULL;
        thread[i] = NULL;

        timer[i] = new QTimer();
        timer[i]->setInterval(500);

        openFlag[i] = false;

    }

    connect(timer[0], SIGNAL(timeout()), this, SLOT(updateReadDataUi1()));

    connect(timer[0], SIGNAL(timeout()), this, SLOT(updateSendCntUi1()));


    localConfigData = (QStringList() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0");

    const QStringList baudRate = (QStringList() << "9600" << "19200" << "38400" << "57600" << "115200" << "Custom");
    const QStringList dataBits = (QStringList() << "5" << "6" << "7" << "8");
    const QStringList stopBits = (QStringList() << "1" << "1.5" << "2");
    const QStringList parity = (QStringList() << "None" << "Even" << "Odd" << "Mark" << "Space");
    const QStringList flowCtrl = (QStringList() << "None" << "RTS/CTS" << "XON/XOFF");


    initIntervalSpinBox(ui->spinBox_11);
    ui->comboBox_11->addItems(baudRate);
    ui->comboBox_12->addItems(dataBits);
    ui->comboBox_13->addItems(parity);
    ui->comboBox_14->addItems(stopBits);
    ui->comboBox_15->addItems(flowCtrl);
    ui->comboBox_12->setCurrentIndex(3);
    initPortUi(ui->btn_open_1,
               ui->spinBox_11,
               ui->checkBox_12,
               ui->btn_send_1,
               ui->label_cnt_11,
               recvCnt[0],
               ui->label_cnt_12,
               sendCnt[0],
               ui->radio_ascii_11,
               ui->radio_ascii_12);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configPort()
{

//    for (int i=0; i<1; i++) {

//        if (worker[0] && worker[0]->isOpened()) {

//            QMessageBox::about(NULL, "警告", "请先关闭所有串口再进行配置");

//            return;
//        }
//    }

    //ConfigDialog *configDialog = new ConfigDialog(this);
    //connect(configDialog, SIGNAL(sendConfigData(QStringList)), this, SLOT(recvConfigData(QStringList)));
    //configDialog->exec();

}

void MainWindow::recvConfigData(QStringList configData)
{

    localConfigData = configData;

    // COM1
    ui->groupBox_11->setTitle(localConfigData[0]);
    ui->groupBox_12->setTitle(localConfigData[0]);
    ui->comboBox_11->setEnabled(true);

    configured_flag = 1;

//    on_btn_close_1_clicked();

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
        ui->comboBox_16->addItem(str);

    }

}

// section send end

// section updateUiOpened

void MainWindow::updateUiOpened(QComboBox *comboBox0,
                                QPushButton* btnOpen,
                                QPushButton* btnSend,
                                QComboBox* comboBox1,
                                QComboBox* comboBox2,
                                QComboBox* comboBox3,
                                QComboBox* comboBox4,
                                QComboBox* comboBox5,
                                QSpinBox* spinBox,
                                QCheckBox* chkBox
                                )
{
    comboBox0->setDisabled(true);
    btnOpen->setText(tr("Close"));
    btnSend->setDisabled(false);
    comboBox1->setDisabled(true);
    comboBox2->setDisabled(true);
    comboBox3->setDisabled(true);
    comboBox4->setDisabled(true);
    comboBox5->setDisabled(true);
    spinBox->setDisabled(false);
    chkBox->setDisabled(false);
}

void MainWindow::updateUiOpened1()
{
    updateUiOpened(ui->comboBox_10,
                   ui->btn_open_1,
                   ui->btn_send_1,
                   ui->comboBox_11,
                   ui->comboBox_12,
                   ui->comboBox_13,
                   ui->comboBox_14,
                   ui->comboBox_15,
                   ui->spinBox_11,
                   ui->checkBox_12);

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMdd-hh_mm_ss_zzz");
    dataFile = new QFile(tr("datafile-")+current_date+tr(".dat"));
    if (!dataFile->open(QIODevice::ReadWrite|QIODevice::Text)) {
        QMessageBox::warning(this, tr("Serial Test"), tr("cannot create data file, quit!"));
        return ;
    }
}

// section updateUiOpened end

// section updateSendCntUi

void MainWindow::updateSendCntUi1()
{
    ui->label_cnt_12->setText(QString::number(sendCnt[0]));
}

// section updateSendCntUi end

void MainWindow::updateSendCnt1(int cnt)
{
    sendCnt[0] = cnt;
}

// section updateSendCntUi end

// section cannotOpenNotify

void MainWindow::cannotOpenNotify1()
{
    QMessageBox::about(NULL, "警告", "打开串口失败");
    openFlag[0] = false;
    emit sigClose1();
}

// section cannotOpenNotify end

// section btn_open

void MainWindow::on_btn_open_1_clicked()
{

//    if (configured_flag == 0) {

//        QMessageBox::about(NULL, "警告", "请先配置串口");
//        return;

//    }

    //QString portName = localConfigData[0];

    if (!worker[0]) {

        worker[0] = new MyWorker();
        worker[0]->setter(ui->comboBox_10->currentText(),
                        ui->comboBox_11->currentText().toLong(),
                        dataBitsLst[ui->comboBox_12->currentIndex()],
                        parityLst[ui->comboBox_13->currentIndex()],
                        stopBitsLst[ui->comboBox_14->currentIndex()],
                        flowCtrlLst[ui->comboBox_15->currentIndex()]
                        );

        thread[0] = new QThread();
        connect(thread[0], SIGNAL(finished()), worker[0], SLOT(deleteLater()));
        connect(worker[0], SIGNAL(sigUpdateSendCnt(int)), this, SLOT(updateSendCnt1(int)));
        connect(worker[0], SIGNAL(sigCannotOpen()), this, SLOT(cannotOpenNotify1()));
        connect(worker[0], SIGNAL(sigExitThread()), this, SLOT(exitThread1()));
        connect(worker[0], SIGNAL(sigOpened()), this, SLOT(updateUiOpened1()));
        connect(worker[0], SIGNAL(sigUpdateReadDataUi(QByteArray)), this, SLOT(storeReadData1(QByteArray)));
        connect(this, SIGNAL(sigOpen1(QString)), worker[0], SLOT(doOpen(QString)));
        connect(this, SIGNAL(sigSend1(QString, bool, int)), worker[0], SLOT(doSend(QString, bool, int)));
        connect(this, SIGNAL(sigClose1()), worker[0], SLOT(doClose()));
        connect(this, SIGNAL(sigContinueSend1(QString, int, int, bool)), worker[0], SLOT(doContinueSend(QString, int, int, bool)));
        connect(ui->textEdit_11, SIGNAL(cursorPositionChanged()), this, SLOT(updateVisibleArea1()));

        worker[0]->moveToThread(thread[0]);
    }

    if (!worker[0]->isOpened()) {
        thread[0]->start();
        timer[0]->start();
        emit sigOpen1(ui->comboBox_10->currentText());

    } else {
        ui->checkBox_12->setCheckState(Qt::Unchecked);

        intervalGen(100);

        worker[0]->continueFlag = false;
        emit sigClose1();
        //openFlag[0] = false;

        timer[0]->stop();

        updateUiClosed(ui->comboBox_10,
                       ui->btn_open_1,
                       ui->btn_send_1,
                       ui->spinBox_11,
                       ui->checkBox_12,
                       ui->comboBox_11,
                       ui->comboBox_12,
                       ui->comboBox_13,
                       ui->comboBox_14,
                       ui->comboBox_15,
                       ui->btn_clr_12,
                       ui->textEdit_12);
    }

//    emit sigOpen1();
//    openFlag[0] = true;

}

// section btn_open end

// section exitThread

void MainWindow::exitThread1()
{
    thread[0]->exit();
    thread[0]->wait();
}

// section exitThread end

// section btn_close
void MainWindow::updateUiClosed(QComboBox* comboBox0,
                                QPushButton* btnOpen,
                                QPushButton* btnSend,
                                QSpinBox* spinBox,
                                QCheckBox* chkBox,
                                QComboBox* comboBox1,
                                QComboBox* comboBox2,
                                QComboBox* comboBox3,
                                QComboBox* comboBox4,
                                QComboBox* comboBox5,
                                QPushButton* btnSendClr,
                                QTextEdit* edit)
{

    comboBox0->setEnabled(true);
    btnOpen->setText(tr("Open"));
    btnSend->setDisabled(true);
    spinBox->setDisabled(true);
    chkBox->setDisabled(true);
    comboBox1->setDisabled(false);
    comboBox2->setDisabled(false);
    comboBox3->setDisabled(false);
    comboBox4->setDisabled(false);
    comboBox5->setDisabled(false);
    btnSendClr->setEnabled(true);
    edit->setReadOnly(false);

    dataFile->flush();
    dataFile->close();
    // if nothing is received before you close the port,
    // then remove this empty file.
    if (dataFile->size() == 0) {
        dataFile->remove();
    }
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
        ui->spinBox_11->setDisabled(false);
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

// section btn_clr end

// section updateVisibleArea

void MainWindow::updateVisibleArea1()
{
    ui->textEdit_11->setTextCursor(ui->textEdit_11->textCursor());
}

// section updateVisibleArea end

void MainWindow::closeEvent(QCloseEvent*)
{

    // COM1
    if (worker[0] != NULL && thread[0] != NULL) {

        worker[0]->continueFlag = false;
        timer[0]->stop();
        emit sigClose1();
    }

}

// section updateReadDataUi

void MainWindow::storeReadData1(QByteArray readData)
{
    tmparr[0].append(readData);
}

// section updateReadDataUi

void MainWindow::updateReadDataUi1()
{

    if (tmparr[0].count() != 0) {

        //dataFile->write(tmparr[0]);

        if (ui->radio_hex_11->isChecked()) {

            int i = 0;

            tmparr[0] = tmparr[0].toHex();

            while (i < tmparr[0].size()) {

                tmparr[0].insert(i, " ");
                i += 3;
                recvCnt[0]++;

            }

        } else {

            recvCnt[0] += tmparr[0].count();

        }

        ui->textEdit_11->insertPlainText(tmparr[0]);
        limitCharsInTextEdit(ui->textEdit_11, MAXCHARS);
        ui->label_cnt_11->setText(QString::number(recvCnt[0]));
        dataFile->write(tmparr[0]);
        tmparr[0].clear();
    }

}

// section updateReadDataUi end

void MainWindow::limitCharsInTextEdit(const QTextEdit* textEdit, const int maxChars)
{

    int currentCount = textEdit->document()->characterCount();

    if(currentCount > (maxChars + (maxChars / 5)))
    {
        QTextCursor tc = textEdit->textCursor();
        tc.setPosition(0);
        tc.setPosition(currentCount - (maxChars + 1), QTextCursor::KeepAnchor);
        tc.removeSelectedText();
        tc.movePosition( QTextCursor::End, QTextCursor::MoveAnchor );
    }
}

void MainWindow::on_actionAbout_triggered()
{

    about = About::getInstance();
    about->show();
    about->activateWindow();
}

void MainWindow::on_actionPreferences_triggered()
{
    configPort();
}

void MainWindow::on_actionNewWindow_triggered()
{
    qDebug() << "new window";
    sub = new MainWindow(this);
    sub->show();
}

void MainWindow::on_comboBox_11_currentIndexChanged(const QString &arg1)
{
    on_comboBox_currentIndexChanged_internal(arg1, ui->comboBox_11);
}

void MainWindow::on_comboBox_currentIndexChanged_internal(const QString currentText, QComboBox *cb)
{
    if (currentText == "Custom") {
        cb->setEditable(true);
        cb->setCurrentText("");
        cb->setFocus();
    } else {
        cb->setEditable(false);
    }
}

void MainWindow::on_comboBox_16_activated(const QString &arg1)
{
    ui->textEdit_12->clear();
    ui->textEdit_12->setText(arg1);
}

void MainWindow::on_comboBox_16_editTextChanged(const QString &arg1)
{
    qDebug() << "edit";
    ui->comboBox_16->setCurrentText(arg1);
}

void MainWindow::on_comboBox_16_currentTextChanged(const QString &arg1)
{
    qDebug() << "change";
}

void MainWindow::on_comboBox_16_highlighted(const QString &arg1)
{
    qDebug() << "hightlight";
}

void MainWindow::on_radio_ascii_12_clicked()
{
}

void MainWindow::on_radio_hex_12_clicked()
{
    QString oldText = ui->textEdit_12->toPlainText();
    char tmp;
    char newTmp[3];
    QString newText;
    QChar tmpText;

    if (oldText.size() == 0) {
        return;
    }

    for (int i=0; i<oldText.size(); i++) {
        tmpText = oldText.at(i);
        tmp = tmpText.toLatin1();
        sprintf(newTmp, "%0x ", tmp);
        newText.append(newTmp);
    }

    newText = newText.trimmed();

    ui->textEdit_12->setPlainText(newText);
}
