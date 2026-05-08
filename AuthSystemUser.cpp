#include "AuthSystemUser.h"
#include "Product.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

AuthSystemUser::AuthSystemUser() {
    load_from_file();
    if (users.empty()) {
        User supadmin;
        supadmin.set_name("SuperAdmin");
        supadmin.set_password("admin***123");
        supadmin.set_status("superadmin");
        users.push_back(supadmin);
        save_to_file(supadmin);
    }
}

bool AuthSystemUser::is_valid_username(const std::string& username) const {
    if (username.size() < 5 || username.size() > 20) return false;

    for (char c : username) {
        if (!std::isalnum(c)) {
            std::cerr << "Имя пользователя может содержать только буквы и цифры" << std::endl;
            return false;
        }
    }
    return true;
}

bool AuthSystemUser::is_valid_pass(const std::string& password) const {
    if (password.size() < 8) {
        std::cerr << "Пароль должен быть не менее 8 символов" << std::endl;
        return false;
    }

    int special_count = 0;
    const std::string special = "!@#$%^&*()_+-=[]{};':\",./<>?\\|`~";

    for (char c : password) {
        if (special.find(c) != std::string::npos) {
            special_count++;
        }
    }

    if (special_count < 3) {
        std::cerr << "Пароль должен содержать минимум 3 специальных символа" << std::endl;
        return false;
    }

    return true;
}

bool AuthSystemUser::user_exists(const std::string& username) const {
    for (const auto& user : users) {
        if (user.username == username) return true;
    }
    return false;
}

void AuthSystemUser::load_from_file() {
    users.clear();
    std::ifstream in_file("Users.txt");
    if (!in_file.is_open()) return;

    std::string line;
    while (std::getline(in_file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        User user;
        std::getline(ss, user.username, '|');
        std::getline(ss, user.password, '|');
        std::getline(ss, user.status);

        users.push_back(user);
    }
    in_file.close();
}

void AuthSystemUser::save_to_file(User& add_user) {
    auto it = std::find(users.begin(), users.end(), add_user);
    if (it == users.end()) {
        users.push_back(add_user);
    }

    std::ofstream out_file("Users.txt", std::ios::trunc);
    if (!out_file.is_open()) return;

    for (const auto& u : users) {
        out_file << u.username << "|" << u.password << "|" << u.status << "\n";
    }
    out_file.close();
}

void AuthSystemUser::register_user() {
    std::string username, password;
    int choice_status, choice_to_continue;

    if (!check_bot.verify()) {
        std::cerr << "Ошибка при проверке капчи" << std::endl;
        return;
    }

    std::cout << "Логин: ";
    Getline(username);

    if (!is_valid_username(username)) return;
    if (user_exists(username)) {
        std::cerr << "Пользователь с таким логином уже существует" << std::endl;
        return;
    }

    std::cout << "Пароль: ";
    Getline(password);
    if (!is_valid_pass(password)) return;

    std::cout << "1) Обычный пользователь\n";
    std::cout << "2) Администратор\n";
    std::cout << "3) Супер-администратор\n";
    std::cout << "Выберите статус: ";
    Getline(choice_status);

    if (choice_status < 1 || choice_status > 3) return;

    std::string statuses[] = {"user", "admin", "superadmin"};

    if (choice_status == 3) {
        is_super_admin = true;
    }

    User add_user;
    add_user.set_name(username);
    add_user.set_password(password);
    add_user.set_status(statuses[choice_status - 1]);

    users.push_back(add_user);
    save_to_file(add_user);
    Logger::log_attempt(username, true);
    std::cout << "Регистрация прошла успешно!" << std::endl;

    std::cout << "Для возврата в меню входа введите 0, для работы со складом введите 1: ";
    Getline(choice_to_continue);

    if (choice_to_continue == 1) {
        start_storage.start();
    }
}

void AuthSystemUser::login() {
    std::string username, password;

    std::cout << "Логин: ";
    Getline(username);
    std::cout << "Пароль: ";
    Getline(password);

    if (!check_bot.verify()) {
        std::cerr << "Ошибка при проверке капчи" << std::endl;
        return;
    }

    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            Logger::log_attempt(username, true);
            std::cout << "Добро пожаловать, " << username << "!" << std::endl;
            start_storage.set_current_user_role(user.status);
            start_storage.start();
            return;
        }
    }

    Logger::log_attempt(username, false);
    std::cout << "Неверный логин или пароль" << std::endl;
    start_storage.start();
}

void AuthSystemUser::show_all_users() {
    for (const auto& user : users) {
        std::cout << "Имя: " << user.username
                  << " | Статус: " << user.status << std::endl;
    }
}

bool AuthSystemUser::is_super_admin_exists() {
    for (const auto& user : users) {
        if (user.status == "superadmin") return true;
    }
    return false;
}

void AuthSystemUser::remove_user() {
    if (!check_bot.verify()) {
        std::cerr << "Ошибка при проверке капчи" << std::endl;
        return;
    }

    if (is_super_admin != true) {
        std::cerr << "Удалять пользователей может только супер-администратор" << std::endl;
        return;
    }

    std::string name, password, status;
    std::cout << "Введите логин пароль статус через пробел: ";
    std::cin >> name >> password >> status;
    std::cin.ignore();

    auto it = std::find_if(users.begin(), users.end(),
                           [&](const User& u) {
                               return u.username == name && u.password == password && u.status == status;
                           });

    if (it != users.end()) {
        users.erase(it);

        std::ofstream out_file("Users.txt", std::ios::trunc);
        for (const auto& u : users) {
            out_file << u.username << "|" << u.password << "|" << u.status << "\n";
        }
        out_file.close();

        std::cout << "Пользователь удалён!" << std::endl;
    } else {
        std::cerr << "Пользователь не найден" << std::endl;
    }
}

void AuthSystemUser::user_pass_change() {
}

void AuthSystemUser::user_status_change() {

}

void AuthSystemUser::change_user() {
    if (!check_bot.verify()) {
        std::cerr << "Ошибка при проверке капчи" << std::endl;
        return;
    }

    if (is_super_admin != true) {
        std::cerr << "Изменять данные пользователей может только супер-администратор" << std::endl;
        return;
    }

    int choice_action;
    std::cout << "1) Изменить пароль\n2) Изменить статус\nВыберите действие: ";
    Getline(choice_action);

    if (choice_action == 1) user_pass_change();
    else if (choice_action == 2) user_status_change();
    else std::cerr << "Неверный выбор" << std::endl;
}
