#ifndef MYWORKER2_H
#define MYWORKER2_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QByteArray>

class MyWorker2 : public QObject
{
    Q_OBJECT
public:
    MyWorker2();
    ~MyWorker2();

    void setter(QString portName, int baudRate);
    void intervalGen(int interval);
    QSerialPort* port;
    bool continueFlag;

private slots:
    void doOpen();
    void readPort();
    void doSend(QString str);
    void doClose();
    void doContinueSend(QString str, int interval, int sendCnt);

signals:
    void dataReady();
    void sigUpdateReadDataUi(QByteArray readData);
    void sigUpdateSendCntUi(int sendCnt);
    void sigExitThread();
    void sigOpened();
    void sigCannotOpen();

private:
    QString portName;
    int baudRate;



};

#endif // MYWORKER2_H
