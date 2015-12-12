#ifndef MYWORKER_H
#define MYWORKER_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QByteArray>

class MyWorker : public QObject
{
    Q_OBJECT
public:
    MyWorker();
    ~MyWorker();

    void setter(QString portName,
                int baudRate,
                QSerialPort::DataBits dataBits,
                QSerialPort::Parity parity,
                QSerialPort::StopBits stopBits,
                QSerialPort::FlowControl flowCtrl);

    void intervalGen(int interval);
    QSerialPort* port;
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
    int baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
    QSerialPort::FlowControl flowCtrl;

};

#endif // MYWORKER_H
