#ifndef LOGGER_H
#define LOGGER_H
#include <string>

class Logger {
public:
    static void log_attempt(const std::string& username, bool success);
};

#endif
