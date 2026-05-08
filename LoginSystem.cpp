#include "LoginSystem.h"
#include "Product.h"

void LoginSystem::launch() {
    int choice_option;

    while (true) {
        std::cout << "1) Регистрация\n";
        std::cout << "2) Авторизация\n";
        std::cout << "3) Смена пользователя\n";
        std::cout << "4) Показать всех пользователей\n";
        std::cout << "5) Выход\n";
        std::cout << "Выберите опцию: ";
        Getline(choice_option);

        switch(choice_option) {
        case 1:
            asu.register_user();
            break;
        case 2:
            asu.login();
            break;
        case 3:
            asu.change_user();
            break;
        case 4:
            asu.show_all_users();
            break;
        case 5:
            std::cout << "Завершение программы" << std::endl;
            return;
        default:
            std::cerr << "Неверный выбор!" << std::endl;
        }
    }
}
