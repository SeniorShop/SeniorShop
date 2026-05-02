#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>

class Logger {
public:
    static void log_attempt(const std::string& username, bool success);
};

#endif // !LOGGER_HPP
