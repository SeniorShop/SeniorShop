#include "LoginSystem.h"
#include "Product.h"
#include <iostream>

void LoginSystem::launch() {
    std::string choose;

    while (true) {
        std::cout << "\n\n\n\t\t\tСИСТЕМА ВХОДА\n\n\n";
        std::cout << "1 - Вход\n";
        std::cout << "2 - Регистрация\n";
        std::cout << "3 - Выход\n";
        std::cout << "Выбор: ";
        Getline(choose);

        if (choose == "1") {
            User* loggerUser = asu.login();
            if (loggerUser) {
                asu.get_storage().start(loggerUser->status, asu);
            }
        } else if (choose == "2") {
            asu.register_user();
        } else if (choose == "3") {
            std::cout << "Завершение программы" << std::endl;
            return;
        } else {
            std::cerr << "Неверный выбор\n";
        }
    }
}
