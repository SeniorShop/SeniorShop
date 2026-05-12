#include "AuthSystemUser.h"
#include "Product.h"
#include <fstream>
#include <sstream>
#include <algorithm>

AuthSystemUser::AuthSystemUser() {
    load_from_file();
    if (users.empty()) {
        users.emplace_back("SuperAdmin", "admin***123", "superadmin");
        save_to_file();
    }
}

bool AuthSystemUser::is_valid_username(const std::string& username) const {
    if (username.size() < 5 || username.size() > 20) return false;
    for (char c : username) {
        if (!std::isalnum(c)) return false;
    }
    return true;
}

bool AuthSystemUser::is_valid_pass(const std::string& password) const {
    if (password.size() < 8) return false;
    int special = 0;
    std::string specials = "!@#$%^&*()_+-=[]{};':\",./<>?\\|`~";
    for (char c : password) {
        if (specials.find(c) != std::string::npos) special++;
    }
    return special >= 3;
}

bool AuthSystemUser::user_exists(const std::string& username) const {
    for (const auto& u : users) {
        if (u.username == username) return true;
    }
    return false;
}

void AuthSystemUser::load_from_file() {
    users.clear();
    std::ifstream file("Users.txt");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        users.push_back(User::deserialize(line));
    }
}

void AuthSystemUser::save_to_file() {
    std::ofstream file("Users.txt", std::ios::trunc);
    if (!file.is_open()) return;
    for (const auto& u : users) {
        file << u.serialize() << "\n";
    }
}

User* AuthSystemUser::login() {
    std::string username, password;
    std::cout << "Логин: ";
    Getline(username);
    std::cout << "Пароль: ";
    Getline(password);

    if (!check_bot.verify()) {
        std::cerr << "Ошибка авторизации (капча)\n";
        return nullptr;
    }

    for (auto& u : users) {
        if (u.username == username && u.password == password) {
            Logger::log_attempt(username, true);
            std::cout << "Добро пожаловать, " << username << "!\n";
            currentUser_ = &u;
            return currentUser_;
        }
    }

    Logger::log_attempt(username, false);
    std::cout << "Неверный логин или пароль\n";
    return nullptr;
}

void AuthSystemUser::register_user() {
    std::string username, password;
    int role_choice;

    std::cout << "Логин: ";
    Getline(username);
    if (!is_valid_username(username)) return;
    if (user_exists(username)) {
        std::cerr << "Пользователь уже существует\n";
        return;
    }

    std::cout << "Пароль: ";
    Getline(password);
    if (!is_valid_pass(password)) return;

    std::cout << "Выберите роль:\n";
    std::cout << "1 - Обычный пользователь\n";
    std::cout << "2 - Администратор\n";
    std::cout << "3 - Супер-администратор\n";
    std::cout << "Выбор: ";
    Getline(role_choice);

    std::string status;
    if (role_choice == 1) status = "user";
    else if (role_choice == 2) status = "admin";
    else if (role_choice == 3) status = "superadmin";
    else {
        std::cerr << "Неверный выбор\n";
        return;
    }

    users.emplace_back(username, password, status);
    save_to_file();
    Logger::log_attempt(username, true);
    std::cout << "Регистрация успешна!\n";
    return;
}

void AuthSystemUser::show_all_users() {
    for (const auto& u : users) {
        std::cout << "Логин: " << u.username << ", Статус: " << u.status << "\n";
    }
}

void AuthSystemUser::remove_user() {
    if (!currentUser_ || currentUser_->status != "superadmin") {
        std::cerr << "Только суперадмин может удалять пользователей\n";
        return;
    }

    std::string username;
    std::cout << "Логин пользователя для удаления: ";
    Getline(username);

    auto it = std::find_if(users.begin(), users.end(),
                           [&](const User& u) { return u.username == username; });

    if (it != users.end()) {
        if (it->status == "superadmin") {
            std::cerr << "Нельзя удалить суперадмина\n";
            return;
        }
        users.erase(it);
        save_to_file();
        std::cout << "Пользователь удалён\n";
    } else {
        std::cerr << "Пользователь не найден\n";
    }
}

void AuthSystemUser::change_user() {
    if (!currentUser_ || currentUser_->status != "superadmin") {
        std::cerr << "Только суперадмин может изменять пользователей\n";
        return;
    }

    std::string username;
    std::cout << "Логин пользователя для изменения: ";
    Getline(username);

    for (auto& u : users) {
        if (u.username == username) {
            std::string new_pass, new_status;
            std::cout << "Новый пароль (оставь пустым, чтобы не менять): ";
            Getline(new_pass);
            if (!new_pass.empty()) {
                if (is_valid_pass(new_pass)) u.password = new_pass;
                else std::cerr << "Пароль не принят\n";
            }

            std::cout << "Новый статус (user/admin, оставь пустым, чтобы не менять): ";
            Getline(new_status);
            if (!new_status.empty() && (new_status == "user" || new_status == "admin")) {
                u.status = new_status;
            }
            save_to_file();
            std::cout << "Данные обновлены\n";
            return;
        }
    }
    std::cerr << "Пользователь не найден\n";
}
