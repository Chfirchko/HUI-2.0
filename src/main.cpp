#include <QCoreApplication>  // для QCoreApplication
#include <QString>           // для QString
#include <QByteArray>        // для QByteArray
#include <QDebug>            // для qWarning и qInfo

#include "serial/SerialPort.h"       // твой класс SerialPort
#include "data/PacketProcessor.h"    // твой класс PacketProcessor
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    SerialPort serial("/dev/ttyUSB0");
    PacketProcessor processor;

    if (!serial.open()) {
        return -1;
    }

    for (int cmdInt = 1; cmdInt <= 6; ++cmdInt) {
        QString cmd = QString::number(cmdInt);
        QByteArray cmdData = cmd.toUtf8() + "\n";
        serial.write(cmdData);

        int expectedSize = processor.getExpectedSize(cmd);
        if (expectedSize <= 0) {
            qWarning() << "Неизвестная команда или размер пакета для команды" << cmd;
            continue;
        }

        QByteArray response = serial.readBytes(expectedSize);
        if (response.isEmpty()) {
            qWarning() << "Нет ответа от устройства для команды" << cmd;
            continue;
        }

        switch (cmdInt) {
            case 1:
                processor.processI2CDevice(response);
                break;
            case 2:
                processor.processI2CDevice(response);
                break;
            case 3:
                processor.processCommand2(response);
                break;
            case 4:
                processor.processCommand2(response);
                break;
            case 5:
                processor.processCommand3(response);
                break;
            case 6:
                processor.processCommand4(response);
                break;
            default:
                qWarning() << "Неизвестная команда:" << cmdInt;
        }
    }

    serial.close();
    return 0;
}
