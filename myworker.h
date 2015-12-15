#ifndef MYWORKER_H
#define MYWORKER_H

#include <QObject>
#include <QString>
#include "qextserialport.h"
#include <QByteArray>

class MyWorker : public QObject
{
    Q_OBJECT
public:
    MyWorker();
    ~MyWorker();

    void setter(QString portName,
                BaudRateType baudRate,
                DataBitsType dataBits,
                ParityType parity,
                StopBitsType stopBits,
                FlowType flowCtrl);

    void intervalGen(int interval);
    QextSerialPort* port;
    bool continueFlag;

private slots:
    void doOpen();
    void readPort();
    void doSend(QString str, bool isHex, int sendCnt);
    void doClose();
    void doContinueSend(QString str, int interval, int sendCnt, bool isHex);

signals:
    void dataReady();
    void sigUpdateReadDataUi(QByteArray readData);
    void sigUpdateSendCnt(int sendCnt);
    void sigExitThread();
    void sigOpened();
    void sigCannotOpen();

private:
    QString portName;
    BaudRateType baudRate;
    DataBitsType dataBits;
    StopBitsType stopBits;
    ParityType parity;
    FlowType flowCtrl;

};

#endif // MYWORKER_H
