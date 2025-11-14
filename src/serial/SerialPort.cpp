#include "serial/SerialPort.h"
#include <QDebug>
#include <qstringview.h>


SerialPort::SerialPort(const QString& portName, QObject* parent)
    : QObject(parent), serial_(this) {
    serial_.setPortName(portName);
    serial_.setBaudRate(QSerialPort::Baud9600);
}
SerialPort::~SerialPort() = default;
bool SerialPort::open() {
    if (serial_.open(QIODevice::ReadWrite)) {
        qInfo() << "Подключено к" << serial_.portName();
        return true;
    } else {
        qCritical() << "Не удалось открыть порт" << serial_.portName() << ":" << serial_.errorString();
        return false;
    }
}

void SerialPort::close() {
    if (serial_.isOpen()) {
        serial_.close();
        qInfo() << "Порт успешно закрыт";
    }
}

bool SerialPort::write(const QByteArray& data) {
    qint64 bytesWritten = serial_.write(data);
    if (bytesWritten == data.size()) {
        qInfo() << "Отправлено:" << data;
        return true;
    } else {
        qWarning() << "Не удалось отправить все данные";
        return false;
    }
}

QByteArray SerialPort::readBytes(int bytesToRead) {
    QByteArray buffer;
    while (buffer.size() < bytesToRead) {
        if (!serial_.waitForReadyRead(500)) {
            qWarning() << "Нет данных для чтения";
            break;
        }
        buffer.append(serial_.read(bytesToRead - buffer.size()));
    }
    return buffer;
}



