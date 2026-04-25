#include <Windows.h>

#include "LoginSystem.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    LoginSystem login_system;
    login_system.launch();
    return 0;
}
