#include "Logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>

void Logger::log_attempt(const std::string& username, bool success) {
    std::ofstream log_file("Logs.txt", std::ios::app);

    if (!log_file.is_open()) {
        std::cerr << "Ошибка открытия файла логов" << std::endl;
        return;
    }

    auto log_time = std::chrono::system_clock::now();
    std::string time_str = std::format("{:%Y-%m-%d %H:%M:%S}", log_time);

    log_file << time_str << " | " << username << " | "
             << (success ? "УСПЕХ" : "НЕУДАЧА") << std::endl;

    log_file.close();
}
