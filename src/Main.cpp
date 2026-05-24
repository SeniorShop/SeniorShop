#include "../include/LoginSystem.h"

#ifdef _WIN32
#include "Windows.h"
#elif __linux__
#include <clocale>
#else
#warning "Unknown header file"
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#elif __linux__
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
#else
#warning "The project has not supporting of the system";
#endif

    LoginSystem login_system;
    login_system.launch();

    return 0;
}
