#include "Logger.h"

void Logger::log_attempt(const std::string& username, bool success) {
    std::ofstream log_file;

    try {
        log_file.open("Logs.txt", std::ios::trunc);
    } catch (...) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    auto log = std::chrono::system_clock::now();
    std::string time_buffer = std::format("{:%Y-%m-%d %H:%M:%S}",log);

    log_file << time_buffer
             << " | Имя пользователя: " << username
             << " | Результат: " << (success ? "true" : "false")
             << std::endl;

    log_file.close();
}
