#include "Logger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

void Logger::LogAttempt(const std::string& _login, bool success)
{
	std::ofstream _logFile("Logs.txt", std::ios::app);
	if (!_logFile.is_open())
	{
		std::cerr << "Ошибка открытия файла Logs.txt\n";
		return;
	}
    std::time_t now = std::time(nullptr); //текущее время в секундах
    std::tm* local_tm = std::localtime(&now); //преобразуем в локальное время
    char timeBuffer[20];//20 byte, "YYYY-MM-DD HH:MM:SS" + null"defualt"
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", local_tm);

    _logFile << timeBuffer
        << " | Пользователь: " << _login
        << " | Результат: " << (success ? "успешно" : "грустно")
        << std::endl;
}