#ifndef LOGGER_HPP
#define LOGGER_HPP

class Logger
{
public:
	static void LogAttempt(const std::string& _login, bool success);
};
#endif // !LOGGER_HPP
