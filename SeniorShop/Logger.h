#ifndef LOGGER_H
#define LOGGER_H
#include "Libraries.h"

class Logger {
public:
    static void log_attempt(const std::string& _login, bool success);
};
#endif // !LOGGER_HPP
