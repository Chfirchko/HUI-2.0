
#pragma once

#include <QString>
#include <QByteArray>

class PacketProcessor {
public:
    void processI2CDevice(const QByteArray& data);

    // Обработка команд
    void processCommand2(const QByteArray& data);
    void processCommand3(const QByteArray& data);
    void processCommand4(const QByteArray& data);
    int getExpectedSize(const QString& cmd) const;
};

