
#include <boost/asio.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>

int main() {
    using namespace boost::asio;

    try {
        io_service io;
        serial_port serial(io, "/dev/ttyUSB2");

        serial.set_option(serial_port_base::baud_rate(9600));
        serial.set_option(serial_port_base::character_size(8));
        serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
        serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        serial.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

        std::cout << "✅ Подключено к /dev/ttyUSB2" << std::endl;

        // Очистим буфер перед отправкой
        serial.cancel();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Отправляем команду
        std::string cmd = "1\n";
        write(serial, buffer(cmd));
        std::cout << "📤 Отправлено: " << cmd;

        // Немного ждём, чтобы устройство успело ответить
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // Считываем всё, что есть
        std::vector<unsigned char> buf(64);
        boost::system::error_code ec;
        size_t n = serial.read_some(buffer(buf), ec);

        if (ec && ec != boost::asio::error::would_block) {
            std::cerr << "❌ Ошибка чтения: " << ec.message() << std::endl;
        } else if (n > 0) {
            std::cout << "📥 Получено (" << n << " байт):" << std::endl;

            std::cout << "HEX: ";
            for (size_t i = 0; i < n; ++i)
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buf[i] << " ";
            std::cout << std::dec << std::endl;

            std::cout << "DEC: ";
            for (size_t i = 0; i < n; ++i)
                std::cout << (int)buf[i] << " ";
            std::cout << std::endl;
        } else {
            std::cout << "⏳ Нет ответа" << std::endl;
        }

        // Закрываем порт
        std::cout << "🔌 Отключение от порта..." << std::endl;
        serial.close();
        std::cout << "✅ Порт успешно закрыт" << std::endl;

    } catch (std::exception &e) {
        std::cerr << "❌ Ошибка: " << e.what() << std::endl;
    }

    return 0;
}

