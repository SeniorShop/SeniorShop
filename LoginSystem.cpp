#include "LoginSystem.h"

void LoginSystem::launch() {
    std::size_t choice_option;
    std::cout << "1) Регистрация\n2) Авторизация\n3) Смена пользователя\n4) Показать всех пользователей\n5) Выход\nВыберите опцию: ";
    std::cin >> choice_option;

    switch(choice_option) {
    case 1:
        registration_button();
        break;
    case 2:
        authorization_button();
        break;
    case 3:
        change_user_button();
        break;
    case 4:
        show_all_users_button();
        break;
    case 5:
        exit_button();
        break;
    }
}

void LoginSystem::registration_button() {
    asu.register_user();
}
void LoginSystem::authorization_button() {
    asu.login();
}
void LoginSystem::change_user_button() {
    asu.change_user();
}
void LoginSystem::show_all_users_button() {
    asu.show_all_users();
}
void LoginSystem::exit_button() {
    std::cout << "Завершение программы" << std::endl;
    exit(0);
}
