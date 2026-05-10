#include "Logger.h"

void Logger::log_attempt(const std::string& username, bool success) {
    std::ofstream log_file("Logs.txt", std::ios::app); //дозапись
    if (!log_file.is_open()) {
        std::cerr << "Ошибка открытия файла логов" << std::endl;
        return;
    }
    auto now = std::chrono::system_clock::now();
    std::string time_buffer = std::format("{:%Y-%m-%d %H:%M:%S}", now);
    log_file << time_buffer
        << " | Имя пользователя: " << username
        << " | Результат: " << (success ? "true" : "false")
        << std::endl;
}
