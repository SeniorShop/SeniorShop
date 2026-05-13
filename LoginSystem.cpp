#include "LoginSystem.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <stdlib.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

void LoginSystem::launch() {
    std::string choose;

    while (true) {
        std::cout << "\n\n\n\t\t\tАпрельское возвращение\n\n\n";
        std::cout << "1 - Вход\n2- Выход\nВыбор: ";
        Getline(choose);
        if (choose == "1") {
            User* loggerUser = asu.login();

            if (loggerUser) {
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                get_start.setAuthSystem(&asu);
                get_start.start(loggerUser->status);
            }
        }
        else if (choose == "2") {
            exit_button();
        }
        else {
            std::cerr << "Неверный выбор!\n";
        }
    }

}

void LoginSystem::exit_button() {
    std::cout << "Завершение программы" << std::endl;
    exit(0);
}
