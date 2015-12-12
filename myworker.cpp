#include "myworker.h"

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QEventLoop>
#include <QTimer>

MyWorker::MyWorker()
{
    continueFlag = false;
}

MyWorker::~MyWorker()
{

}

void MyWorker::doOpen()
{
    qDebug() << "doOpen";

    port = new QSerialPort(portName);

    port->setBaudRate(baudRate);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    if (port->open(QIODevice::ReadWrite)) {

        emit sigOpened();
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

        emit sigUpdateSendCnt(sendCnt);
        intervalGen(interval);

    }

}

void MyWorker::intervalGen(int interval)
{
    QEventLoop loop;
    QTimer::singleShot(interval, &loop, SLOT(quit()));
    loop.exec();
}

void MyWorker::setter(QString portName, int baudRate)
{
    this->portName = portName;
    this->baudRate = baudRate;
}

void MyWorker::readPort()
{

    QByteArray readData = port->readAll();
    emit sigUpdateReadDataUi(readData);

}

void MyWorker::doClose()
{
    if (port != NULL && port->isOpen()) {

        port->close();

    }

    if (port != NULL) {

        emit sigExitThread();

    }
    qDebug() << "close";
}
