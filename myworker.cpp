#include "myworker.h"

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QEventLoop>
#include <QTimer>
#include <math.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

MyWorker::MyWorker()
{
    continueFlag = false;
    openFlag = false;
}

MyWorker::~MyWorker()
{

}

void MyWorker::doOpen(QString portName)
{
    qDebug() << "doOpen";

    port = new QSerialPort(portName);

    port->setBaudRate(baudRate);
    port->setDataBits(dataBits);
    port->setParity(parity);
    port->setStopBits(stopBits);
    port->setFlowControl(flowCtrl);

    if (port->open(QIODevice::ReadWrite)) {

        emit sigOpened();
        openFlag = true;
        connect(port, SIGNAL(readyRead()), this, SLOT(readPort()));

    } else {
        emit sigCannotOpen();
    }

}

void MyWorker::doSend(QString str, bool isHex, int sendCnt)
{

//    QString subStr;
//    int dec;
    QByteArray sendArr;
    str = str.replace(" ", "");

    if (isHex) {

//        int i = 0;

//        while (i<str.size()) {

//            subStr = str.mid(i, 2);

//            dec = subStr.toInt(0, 16);

//            qDebug() << QString("%1").arg(dec, 0, 16);

//            i = i+2;

//        }

        //sendCnt += (i != 0) ? (i/2) : 0;
        sendCnt += ceil(str.size()/2.0);
        sendArr =  QByteArray::fromHex(str.toLocal8Bit());
        port->write(sendArr);

    } else {

        sendCnt += str.size();
        port->write(str.toLocal8Bit());

    }

    emit sigUpdateSendCnt(sendCnt);

}

void MyWorker::doContinueSend(QString str, int interval, int sendCnt, bool isHex)
{
    continueFlag = true;

    str = str.replace(" ", "");

//    int i = 0;
//    int j = 0;
//    int dec;
//    QString subStr;
    QByteArray sendArr;

//    if (isHex) {

//        while (i+1<str.size()) {

//            subStr = str.mid(i, 2);

//            dec = subStr.toInt(0, 16);

//            //qDebug() << QString("%1").arg(dec, 0, 16);

//            i = i+2;
//            j++;

//        }
//    }

    while (continueFlag == true) {

        if (isHex) {

            sendArr =  QByteArray::fromHex(str.toLocal8Bit());
            port->write(sendArr);
            //sendCnt += (i != 0) ? (i/2) : 0;
            sendCnt += ceil(str.size()/2.0);

        } else {

            port->write(str.toLocal8Bit());
            sendCnt += str.size();

        }

        // wait for all the bytes written before next write
        // -1 means wait forever.
        port->waitForBytesWritten(-1);
        emit sigUpdateSendCnt(sendCnt);

#ifdef Q_OS_WIN
        intervalGenUs(interval);
#else
        intervalGen(interval);
#endif
    }

}

void MyWorker::intervalGen(int interval)
{
    QEventLoop loop;
    QTimer::singleShot(interval, &loop, SLOT(quit()));
    loop.exec();
}

#ifdef Q_OS_WIN
void MyWorker::intervalGenUs(int interval)
{
    LARGE_INTEGER litmp;
    LONGLONG QPart1, QPart2;
    double dfMinus, dfFreq, dfTim;

    if (interval < 1) {
        return;
    }

    QueryPerformanceFrequency(&litmp);
    dfFreq = (double)litmp.QuadPart;

    QueryPerformanceCounter(&litmp);
    QPart1 = litmp.QuadPart;

    do {
        QueryPerformanceCounter(&litmp);
        QPart2 = litmp.QuadPart;
        dfMinus = (double)(QPart2-QPart1);
        dfTim = dfMinus * 1000000 / dfFreq;

    } while (dfTim < interval);
}
#endif

bool MyWorker::isOpened()
{
    return openFlag;
}

void MyWorker::setter(QString portName,
                      int baudRate,
                      QSerialPort::DataBits dataBits,
                      QSerialPort::Parity parity,
                      QSerialPort::StopBits stopBits,
                      QSerialPort::FlowControl flowCtrl)
{
    this->portName = portName;
    this->baudRate = baudRate;
    this->dataBits = dataBits;
    this->parity = parity;
    this->stopBits = stopBits;
    this->flowCtrl = flowCtrl;
}

void MyWorker::readPort()
{

    QByteArray readData = port->readAll();
    emit sigUpdateReadDataUi(readData);

}

void MyWorker::doClose()
{
    qDebug() << "doClose";

    if (port != NULL && port->isOpen()) {

        port->close();
        openFlag = false;

    }

    if (port != NULL) {

        //emit sigExitThread();

    }

}
