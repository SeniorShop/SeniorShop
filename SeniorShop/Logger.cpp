#include "Logger.h"

void Logger::log_attempt(const std::string& _login, bool success) {
    std::ofstream log_file("Logs.txt", std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "������ �������� ����� Logs.txt" << std::endl;
        return;
    }
    std::time_t now = std::time(nullptr);
    std::tm* local_tm = std::localtime(&now);
    char timeBuffer[20];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", local_tm);

    log_file << timeBuffer
             << " | ������������: " << _login
             << " | ���������: " << (success ? "�������" : "�������")
             << std::endl;
}
