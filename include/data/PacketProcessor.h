#pragma once

#include <QString>

class PacketProcessor {
public:
    void processI2CDevice(const QByteArray& data);
};

