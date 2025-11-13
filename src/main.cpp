
#include <QCoreApplication>
#include "serial/SerialPort.h"
#include "data/PacketProcessor.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    SerialPort serial("/dev/ttyUSB2");
    PacketProcessor processor;

    if (!serial.open()) {
        return -1;
    }

    for (int cmd = 1; cmd <= 6; ++cmd) {
        QByteArray cmdData = QByteArray::number(cmd) + "\n";
        serial.write(cmdData);

        QByteArray response = serial.readBytes(21);
        if (response.isEmpty()) {
            qWarning() << "⏳ Нет ответа от устройства";
            continue;
        }

        processor.processI2CDevice(response);
    }

    serial.close();
    return 0;
}

