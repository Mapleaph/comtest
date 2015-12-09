#include "myworker2.h"

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QEventLoop>
#include <QTimer>

MyWorker2::MyWorker2()
{
    continueFlag = false;
}

MyWorker2::~MyWorker2()
{

}

void MyWorker2::doOpen()
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

void MyWorker2::doSend(QString str)
{
    qDebug() << str;
    port->write(str.toLocal8Bit());
}

void MyWorker2::doContinueSend(QString str, int interval, int sendCnt)
{
    continueFlag = true;
    qDebug() << "ContinueSend";
    qDebug() << str;
    while (continueFlag == true) {
        port->write(str.toLocal8Bit());
        sendCnt += str.size();
        emit sigUpdateSendCntUi(sendCnt);
        intervalGen(interval);
    }
}

void MyWorker2::intervalGen(int interval)
{
    QEventLoop loop;
    QTimer::singleShot(interval, &loop, SLOT(quit()));
    loop.exec();
}

void MyWorker2::setter(QString portName, int baudRate)
{
    this->portName = portName;
    this->baudRate = baudRate;
}

void MyWorker2::readPort()
{
    QByteArray readData = port->readAll();
    emit sigUpdateReadDataUi(readData);
}

void MyWorker2::doClose()
{
    if (port != NULL && port->isOpen()) {

        port->close();

    }

    if (port != NULL) {

        emit sigExitThread();

    }
    qDebug() << "close";
}
