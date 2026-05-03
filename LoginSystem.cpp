#include "LoginSystem.h"

void LoginSystem::launch() {
    std::size_t choice_option;
    std::cout << "1) Регистрация\n2) Авторизация\n3) Смена пользователя\n4) Удаление пользователя\n5) Показать всех пользователей\n6) Выход\nВыберите опцию: ";
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
        delete_user_button();
        break;
    case 5:
        show_all_users_button();
        break;
    case 6:
        exit_button();
        break;
    }
}

void LoginSystem::registration_button() {
    set_status = asu.register_user();
    return;
}
void LoginSystem::authorization_button() {
    asu.login();
}
void LoginSystem::change_user_button() {
    asu.change_user(set_status);
    return;
}
void LoginSystem::delete_user_button() {
    asu.remove_user(set_status);
    return;
}
void LoginSystem::show_all_users_button() {
    asu.show_all_users();
    return;
}
void LoginSystem::exit_button() {
    std::cout << "Завершение программы" << std::endl;
    exit(0);
}
