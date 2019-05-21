#ifndef SER_H
#define SER_H

#include <QSerialPort>

#define TIME_INTERVAL_MAX 1000000
#define TIME_INTERVAL_MIN 1
#define TIME_INTERVAL_STEP 1
#define TIME_INTERVAL_DEFAULT 1

#define CONFIG_LIST_SIZE 8

int baudRateLst[] = {
    9600, 19200, 38400, 57600, 115200
};

QSerialPort::DataBits dataBitsLst[] = {
    QSerialPort::Data5,
    QSerialPort::Data6,
    QSerialPort::Data7,
    QSerialPort::Data8
};

QSerialPort::Parity parityLst[] = {
    QSerialPort::NoParity,
    QSerialPort::EvenParity,
    QSerialPort::OddParity,
    QSerialPort::SpaceParity,
    QSerialPort::MarkParity,
};

QSerialPort::FlowControl flowCtrlLst[] = {
    QSerialPort::NoFlowControl,
    QSerialPort::HardwareControl,
    QSerialPort::SoftwareControl
};

QSerialPort::StopBits stopBitsLst[] = {
    QSerialPort::OneStop,
    QSerialPort::OneAndHalfStop,
    QSerialPort::TwoStop
};

#endif // SER_H
