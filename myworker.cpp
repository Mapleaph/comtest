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

void MyWorker::doSend(QString str)
{
    qDebug() << str;
    port->write(str.toLocal8Bit());
}

void MyWorker::doContinueSend(QString str, int interval, int sendCnt)
{
    continueFlag = true;
    qDebug() << "ContinueSend";
    qDebug() << str;
    while (continueFlag == true) {
        port->write(str.toLocal8Bit());
        sendCnt += str.size();
        intervalGen(interval);
        emit sigUpdateSendCntUi(sendCnt);
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
