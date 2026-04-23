#include "AuthSystemUser.h"
#include <cctype>
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
    for (char check_symbols : username) {
        if (!std::isalnum(static_cast<unsigned char>(check_symbols)))
            return false;
    }
    return true;
}

bool AuthSystemUser::is_valid_pass(const std::string& password) const {
    if(password.size() < 8) return false;
    std::size_t count_unique_symbols = 0;
    for(char check_symbols : password) {
        if(!std::isalnum(static_cast<unsigned char>(check_symbols)))
            count_unique_symbols++;
    }
    if(count_unique_symbols < 3)
        return false;
    return true;
}

bool AuthSystemUser::user_exists(const std::string& username) const {
    for (const auto& user : users) {
        if (user.username == username) return true;
    }
    return false;
}

bool AuthSystemUser::is_valid_current_status(const std::string& current_status) const {
    return current_status == "superadmin" || current_status == "admin" || current_status == "user";
}

void AuthSystemUser::load_from_file() {
    users.clear();
    std::ifstream in_file("Users.txt");
    if (!in_file.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    std::string every_data;
    User user;
    while (std::getline(in_file, every_data)) {
        if (every_data.empty()) continue;
        std::istringstream ss(every_data);

        if (std::getline(ss, user.username, '|')
            && (std::getline(ss, user.password, '|'))
            && (std::getline(ss, user.status))) {
            users.push_back(user);
        }
    }
    in_file.close();
}

void AuthSystemUser::save_to_file(User& add_user) {
    std::ofstream out_file("Users.txt", std::ios::trunc);
    if (!out_file.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    for (const auto& u : users) {
        out_file << u.username << "|" << u.password << "|" << u.status << std::endl;  // Используйте | как разделитель
    }
    out_file.close();
}

bool AuthSystemUser::register_user() {
    std::string username, password;
    std::size_t choice_status;

    std::cout << "Логин: ";
    std::cin >> username;

    if (!is_valid_username(username)) {
        std::cout << "Неверный логин (должен быть 5-20 символов, только буквы и цифры)" << std::endl;
        return false;
    }

    if (user_exists(username)) {
        std::cout << "Пользователь с таким логином уже существует" << std::endl;
        return false;
    }

    std::cout << "Пароль: ";
    std::cin >> password;

    if (!is_valid_pass(password)) {
        std::cout << "Неверный пароль (минимум 8 символов, минимум 3 не буквенно-цифровых символа)" << std::endl;
        return false;
    }

    std::string statuses[]{"user", "admin", "superadmin"};
    std::cout << "1) Обычный пользователь\n2) Администратор\n3) Супер-администратор\nВыберите статус: ";
    std::cin >> choice_status;

    if(choice_status < 1 || choice_status > 3) return false;

    User add_user;
    add_user.username = username;
    add_user.password = password;
    add_user.status = statuses[choice_status - 1];
    users.push_back(add_user);
    save_to_file(add_user);
    Logger::log_attempt(username, true);
    return true;
}

void AuthSystemUser::show_all_users() {
    for (const auto& user : users) {
        std::cout << "Имя: " << user.username
                  << " | Статус: " << user.status << std::endl;
    }
}

bool AuthSystemUser::login() {
    std::string username, password;
    std::cout << "Логин: ";
    std::cin >> username;
    std::cout << "Пароль: ";
    std::cin >> password;

    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            Logger::log_attempt(username, true);
            std::cout << "Добро пожаловать, " << username << "!" << std::endl;
            return true;
        }
    }

    Logger::log_attempt(username, false);
    std::cout << "Неверный логин или пароль" << std::endl;
    return false;
}

bool AuthSystemUser::is_super_admin_exists() {
    for (const auto& user : users) {
        if (user.status == "superadmin")
            return true;
    }
    return false;
}

bool AuthSystemUser::change_user() {
    std::string name, password, status, new_name, new_password, new_status;
    std::cout << "Логин: ";
    std::cin >> name;
    std::cout << "Пароль: ";
    std::cin >> password;
    std::cout << "Текущий статус: ";
    std::cin >> status;

    for (auto& user : users) {
        if (user.username == name && user.password == password && user.status == status) {
            std::cout << "Новый логин: ";
            std::cin >> new_name;
            std::cout << "Новый пароль: ";
            std::cin >> new_password;
            std::cout << "Новый статус: ";
            std::cin >> new_status;

            user.username = new_name;
            user.password = new_password;
            user.status = new_status;

            save_to_file(user);
            return true;
        }
    }

    std::cout << "Пользователь не найден" << std::endl;
    return false;
}
