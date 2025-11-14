
#include "data/PacketProcessor.h"
#include <QDebug>
#include <QByteArray>
#include <QDataStream>

struct I2CDevicePacket {
    quint8 i2c_addr;
    quint32 mod_id;
    quint32 build_time;
    quint32 proto_ver;
    quint64 serial;
};

int PacketProcessor::getExpectedSize(const QString& cmd) const {
    if (cmd == "1" || cmd == "2") {
        return 21;  // I2CDevice
    } else if (cmd == "3" || cmd == "4") {
        return 16;  // COC
    } else if (cmd == "5" || cmd == "6") {
        return 20;  // COC variants
    } else if (cmd == "A") {
        return 10;  // I2CDevice variant
    } else {
        return -1;  // неизвестный режим
    }
}

void PacketProcessor::processI2CDevice(const QByteArray& data) {
    if (data.size() != 21) {
        qWarning() << QString("I2CDevice: получен пакет %1 байт (ожидалось 21)").arg(data.size());
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

// --- Команда 2 ---
void PacketProcessor::processCommand2(const QByteArray& data) {
    if (data.size() != 16) {
        qWarning() << QString("COC: получен пакет %1 байт (ожидалось 16)").arg(data.size());
        return;
    }

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint32 temp_raw, op_time, current, volt;
    stream >> temp_raw >> op_time >> current >> volt;

    double voltage = ((volt * 1200.0) / 4095.0) / 1000.0;
    int voltage_display = ((volt * 1200) / 4095) % 1000;

    qInfo() << QString(
        "DeviceInfo (16 байт)\n"
        "   Температура: %1 °C\n"
        "   Время работы: %2 тактов\n"
        "   Сила тока: %3 A\n"
        "   ADC[1]: %4 (V = %5)\n"
        "   Напряжение: %6 V")
        .arg(temp_raw / 100.0, 0, 'f', 2)
        .arg(op_time)
        .arg(current)
        .arg(volt)
        .arg(voltage, 0, 'f', 3)
        .arg(voltage_display);
}

// --- Команда 3 ---
void PacketProcessor::processCommand3(const QByteArray& data) {
    if (data.size() != 20) {
        qWarning() << QString("peripheral_volt: получен пакет %1 байт (ожидалось 20)").arg(data.size());
        return;
    }

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint32 volt1, volt2, volt3, volt4, volt5;
    stream >> volt1 >> volt2 >> volt3 >> volt4 >> volt5;

    auto formatVolt = [](quint32 v) {
        return QString::number(((v * 1200) / 4095) % 1000) + " V";
    };

    qInfo() << QString(
        "peripheral_volt (20 байт)\n"
        "   Напряжение: %1\n"
        "   Напряжение: %2\n"
        "   Напряжение: %3\n"
        "   Напряжение: %4\n"
        "   Напряжение: %5")
        .arg(formatVolt(volt1))
        .arg(formatVolt(volt2))
        .arg(formatVolt(volt3))
        .arg(formatVolt(volt4))
        .arg(formatVolt(volt5));
}

// --- Команда 4 ---
void PacketProcessor::processCommand4(const QByteArray& data) {
    if (data.size() != 20) {
        qWarning() << QString("peripheral_temp: получен пакет %1 байт (ожидалось 20)").arg(data.size());
        return;
    }

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint32 temps[5];
    for (int i = 0; i < 5; ++i) {
        stream >> temps[i];
    }

    qInfo() << QString(
        "peripheral_temp (20 байт)\n"
        "   Температура: %1 °C\n"
        "   Температура: %2 °C\n"
        "   Температура: %3 °C\n"
        "   Температура: %4 °C\n"
        "   Температура: %5 °C")
        .arg(temps[0] / 100.0, 0, 'f', 2)
        .arg(temps[1] / 100.0, 0, 'f', 2)
        .arg(temps[2] / 100.0, 0, 'f', 2)
        .arg(temps[3] / 100.0, 0, 'f', 2)
        .arg(temps[4] / 100.0, 0, 'f', 2);
}

