#pragma once

#include <QObject>
#include <QSerialPort>

class SerialPort : public QObject {
    Q_OBJECT
public:
    explicit SerialPort(const QString& portName, QObject* parent = nullptr);
~SerialPort() override;
    bool open();
    void close();

    bool write(const QByteArray& data);
    QByteArray readBytes(int bytesToRead);

private:
    QSerialPort serial_;
};

