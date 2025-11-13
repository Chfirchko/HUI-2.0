#include "data/PacketProcessor.h"
#include <QDebug>

struct I2CDevicePacket {
    quint8 i2c_addr;
    quint32 mod_id;
    quint32 build_time;
    quint32 proto_ver;
    quint64 serial;
};

void PacketProcessor::processI2CDevice(const QByteArray& data) {
    if (data.size() != 21) {
        qWarning() << QString("⚠️ I2CDevice: получен пакет %1 байт (ожидалось 21)").arg(data.size());
        return;
    }

    I2CDevicePacket packet;
    memcpy(&packet.i2c_addr, data.constData(), 1);
    memcpy(&packet.mod_id, data.constData() + 1, 4);
    memcpy(&packet.build_time, data.constData() + 5, 4);
    memcpy(&packet.proto_ver, data.constData() + 9, 4);
    memcpy(&packet.serial, data.constData() + 13, 8);

    qInfo() << QString("I2CDevice (21 байт)\n"
                      "   Адрес I2C: 0x%1 (%2)\n"
                      "   ID модуля: %3\n"
                      "   Время сборки: %4\n"
                      "   Версия протокола: %5\n"
                      "   Серийный номер: %6")
                 .arg(packet.i2c_addr, 2, 16, QChar('0'))
                 .arg(packet.i2c_addr)
                 .arg(packet.mod_id)
                 .arg(packet.build_time)
                 .arg(packet.proto_ver)
                 .arg(packet.serial);
}

