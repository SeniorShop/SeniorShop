#include "../include/AuthSystemUser.h"
#include <filesystem>
#include <algorithm>
#include <unordered_set>
#include <thread>
#ifdef _WIN32
#include "Windows.h"
#else
#include <stdlib.h>
#endif


AuthSystemUser::AuthSystemUser() {
    load_from_file();
    bool superExists = false;
    for (const auto& u : users) {
        if (u->status == "superadmin") {
            superExists = true;
            break;
        }
    }
    if (!superExists) {
        users.push_back(std::make_unique<SuperAdminUser>());
        save_to_file();
    }
}

void AuthSystemUser::load_from_file() {
    users.clear();
    std::ifstream in_file("Users.txt");
    if (!in_file.is_open()) {
        return;
    }
    std::string line;
    while (std::getline(in_file, line)) {
        if (line.empty()) continue;
        auto user = User::deserialize(line);
        if (user) {
            users.push_back(std::move(user));
        }
    }
    in_file.close();
}


void AuthSystemUser::save_to_file() {
    std::ofstream out_file("Users.txt", std::ios::trunc);
    if (!out_file.is_open()) {
        return;
    }
    for (const auto& u : users) {
        out_file << u->serialize() << '\n';
    }
    out_file.close();
}



bool AuthSystemUser::is_valid_username(const std::string& username) const {
    if (username.size() < 5 || username.size() > 20) return false;

    std::unordered_set<char> allowed;
    for (char c = 'A'; c <= 'Z'; ++c) allowed.insert(c);
    for (char c = 'a'; c <= 'z'; ++c) allowed.insert(c);
    for (char c = '0'; c <= '9'; ++c) allowed.insert(c);

    for (char c : username) {
        if (!allowed.count(c)) {
            std::cerr << "Имя пользователя содержит недопустимый символ" << std::endl;
            return false;
        }
    }
    return true;
}


bool AuthSystemUser::is_valid_pass(const std::string& password) const {
    if (password.size() < 8) return false;


    std::unordered_set<char> allowed;
    for (char c = '!'; c <= '~'; ++c) allowed.insert(c);

    for (char c : password) {
        if (!allowed.count(c)) {
            std::cerr << "Некорректные символы в пароле" << std::endl;
            return false;
        }
    }

    std::unordered_set<char> specials = {
        '!','@','#','%','^','&','*','(',')','-','_','=','+','/','?',
        '|','\\','\"','\'',',','.','>','<','~','`',':',';','{','}','[',']'
    };

    size_t specialCount = 0;
    for (char c : password) {
        if (specials.count(c)) ++specialCount;
    }
    if (specialCount < 3) {
        std::cerr << "Пароль должен содержать не менее 3 специальных символов" << std::endl;
        return false;
    }
    return true;
}


bool AuthSystemUser::user_exists(const std::string& username) const {
    for (const auto& u : users) {
        if (u->username == username) return true;
    }
    return false;
}

User* AuthSystemUser::login() {
    std::string username, password;

    while (true) {
        std::cout << "Введите логин: ";
        Getline(username);

        std::size_t start = username.find_first_not_of(" \t\n\r\f\v");
        if (start != std::string::npos) {
            username = username.substr(start);
        }
        std::size_t end = username.find_last_not_of(" \t\n\r\f\v");
        if (end != std::string::npos) {
            username = username.substr(0, end + 1);
        }

        if (!username.empty()) break;
        std::cerr << "Логин не может быть пустым\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    while (true) {
        std::cout << "Введите пароль: ";
        Getline(password);

        std::size_t start = password.find_first_not_of(" \t\n\r\f\v");
        if (start != std::string::npos) {
            password = password.substr(start);
        }
        std::size_t end = password.find_last_not_of(" \t\n\r\f\v");
        if (end != std::string::npos) {
            password = password.substr(0, end + 1);
        }

        if (!password.empty()) break;
        std::cerr << "Пароль не может быть пустым\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    if (!check_bot.verify()) {
        std::cerr << "Ошибка авторизации (капча)" << std::endl;
        return nullptr;
    }

    for (auto& u : users) {
        if (u->username == username && u->password == password) {
            Logger::log_attempt(username, true);
            std::cout << "Добро пожаловать, " << username << "!" << std::endl;
            currentUser_ = u.get();
            return currentUser_;
        }
    }

    Logger::log_attempt(username, false);
    std::cout << "Неверный логин или пароль" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return nullptr;
}


void AuthSystemUser::register_user() {
    if (!currentUser_ || currentUser_->status != "superadmin") {
        std::cerr << "Только суперадминистратор может создавать новых пользователей" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::string username, password;
    std::cout << "Введите логин нового пользователя: ";
    Getline(username);

    if (!is_valid_username(username)) {
        std::cerr << "Логин должен быть от 5 до 20 символов, только буквы и цифры" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    if (user_exists(username)) {
        std::cerr << "Пользователь с таким логином уже существует" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    std::cout << "Введите пароль: ";
    Getline(password);

    if (!is_valid_pass(password)) {
        std::cerr << "Пароль должен быть не менее 8 символов и содержать минимум 3 спецсимвола" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    std::cout << "Выберите статус:\n1 - Обычный пользователь\n2 - Администратор\nВыбор: ";
    std::string choose;
    Getline(choose);

    if (choose != "1" && choose != "2") {
        std::cerr << "Ошибка! Неверный выбор\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }


    std::unique_ptr<User> newUser;
    if (choose == "1") {
        newUser = std::make_unique<RegularUser>(username, password);

    }
    else if (choose == "2") {
        newUser = std::make_unique<AdminUser>(username, password);
    }

    users.push_back(std::move(newUser));
    save_to_file();
    Logger::log_attempt(username, true);
    std::cout << "Пользователь успешно зарегистрирован." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void AuthSystemUser::show_all_users() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    std::cout << "\n\n\n\t\t\tСПИСОК ПОЛЬЗОВАТЕЛЕЙ\n\n\n";
    std::cout << std::left
              << std::setw(25) << "Логин"
              << std::setw(15) << "Статус"
              << "\n";
    std::cout << std::string(40, '-') << "\n";

    for (const auto& u : users) {
        std::cout << std::left
                  << std::setw(25) << u->username
                  << std::setw(15) << u->status
                  << "\n";
    }
    std::cout << std::string(40, '-') << "\n";

    std::cout << "Нажмите Enter для продолжения: ";
    std::cin.get();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void AuthSystemUser::change_user() {
#ifdef _WIN32
        system("cls");
#else
    system("clear");
#endif
    if (!currentUser_ || currentUser_->status != "superadmin") {
        std::cerr << "Только superadmin может изменять пользователей" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    std::cout << "1. Изменить пароль\n2. Изменить статус\n3. Возвращение к меню редактирования персонала\nВыбор: ";
    std::string choose;
    Getline(choose);

    if (choose == "1") {
        user_pass_change();
    }
    else if (choose == "2") {
        user_status_change();
    } else if (choose == "3") {
        return;
    }
    else {
        std::cerr << "Ошибка! Неверный выбор" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

}

void AuthSystemUser::user_pass_change() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::string name, password, status, new_password;
    std::cout << "Введите логин пользователя: ";
    Getline(name);
    std::cout << "Введите текущий пароль: ";
    Getline(password);
    std::cout << "Введите текущий статус: ";
    Getline(status);

    bool success = false;

    for (auto& u : users) {
        if (u->username == name && u->password == password && u->status == status) {
            std::cout << "Введите новый пароль: ";
            Getline(new_password);
            if (!is_valid_pass(new_password)) {
                std::cerr << "Новый пароль не соответствует требованиям" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                break;
            }
            u->password = new_password;
            save_to_file();
            std::cout << "Пароль успешно изменён." << std::endl;
            success = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            break;
        }
    }

    if(!success) {
        std::cerr << "Пользователь не найден или данные не совпадают" << std::endl;
#ifdef _WIN32
        Sleep(1000);
        system("cls");
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        system("clear");
#endif
    }
    return;
}

void AuthSystemUser::user_status_change() {
    std::string name, password, status, new_status;
    std::cout << "Введите логин пользователя: ";
    Getline(name);
    std::cout << "Введите пароль: ";
    Getline(password);
    std::cout << "Введите текущий статус: ";
    Getline(status);

    bool success = false;

    for (auto& u : users) {
        if (u->username == name && u->password == password && u->status == status) {
            if (u->status == "superadmin") {
                std::cerr << "Нельзя изменить статус суперадминистратора" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                break;
            }
            std::cout << "Введите новый статус (admin/user): ";
            Getline(new_status);
            if (new_status != "admin" && new_status != "user") {
                std::cerr << "Недопустимый статус" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                break;
            }
            u->status = new_status;
            save_to_file();
            std::cout << "Статус успешно изменён." << std::endl;
            success = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            break;
        }
    }

    if(!success) {
        std::cerr << "Пользователь не найден или данные не совпадают" << std::endl;
#ifdef _WIN32
        Sleep(2000);
        system("cls");
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        system("clear");
#endif
    }
    return;
}


void AuthSystemUser::remove_user() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    if (!currentUser_ || currentUser_->status != "superadmin") {
        std::cerr << "Только суперадминистратор может удалять пользователей" << std::endl;
        return;
    }

    std::string name, password, status;
    std::cout << "Введите логин пользователя: ";
    Getline(name);
    std::cout << "Введите пароль: ";
    Getline(password);
    std::cout << "Введите статус: ";
    Getline(status);

    bool success = false;

    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->username == name && (*it)->password == password && (*it)->status == status) {
            if ((*it)->status == "superadmin") {
                std::cerr << "Нельзя удалить суперадминистратора" << std::endl;
                break;
            }
            users.erase(it);
            save_to_file();
            std::cout << "Пользователь успешно удалён." << std::endl;
            success = true;
            break;
        }
    }

    if(!success) {
        std::cerr << "Пользователь не найден или данные не совпадают" << std::endl;
#ifdef _WIN32
        Sleep(1000);
        system("cls");
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        system("clear");
#endif
    }
    return;
}

std::string AuthSystemUser::get_all_users_JSON() const {
    std::string json = "[";
    bool first = true;

    for (const auto& u : users) {
        if (!first) json += ",";
        first = false;

        json += "{";
        json += "\"username\":\"" + u->username + "\",";
        json += "\"status\":\"" + u->status + "\"";
        json += "}";
    }

    json += "]";
    return json;
}

bool AuthSystemUser::register_user_direct(const std::string& username,
                                          const std::string& password,
                                          const std::string& role) {
    if (!is_super_admin()) {
        std::cerr << "Только суперадминистратор может создавать пользователей\n";
        return false;
    }

    std::string user = username;
    std::string pass = password;

    auto trim = [](std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        if (start != std::string::npos) {
            str = str.substr(start);
        }
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        if (end != std::string::npos) {
            str = str.substr(0, end + 1);
        }
    };

    trim(user);
    trim(pass);

    if (!is_valid_username(user)) {
        std::cerr << "Логин должен быть от 5 до 20 символов, только буквы и цифры\n";
        return false;
    }

    if (user_exists(user)) {
        std::cerr << "Пользователь уже существует\n";
        return false;
    }

    if (!is_valid_pass(pass)) {
        std::cerr << "Пароль должен быть не менее 8 символов и содержать минимум 3 спецсимвола\n";
        return false;
    }

    if (role != "user" && role != "admin") {
        std::cerr << "Неверная роль. Должно быть 'user' или 'admin'\n";
        return false;
    }

    std::unique_ptr<User> new_user;
    if (role == "admin") {
        new_user = std::make_unique<AdminUser>(user, pass);
    } else {
        new_user = std::make_unique<RegularUser>(user, pass);
    }

    users.push_back(std::move(new_user));
    save_to_file();

    return true;
}

bool AuthSystemUser::delete_user_direct(const std::string& username) {
    if (!is_super_admin()) {
        std::cerr << "Только суперадминистратор может удалять пользователей\n";
        return false;
    }

    if (currentUser_ && currentUser_->username == username) {
        std::cerr << "Нельзя удалить самого себя\n";
        return false;
    }

    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->username == username) {
            if ((*it)->status == "superadmin") {
                std::cerr << "Нельзя удалить суперадминистратора\n";
                return false;
            }
            users.erase(it);
            save_to_file();
            return true;
        }
    }

    std::cerr << "Пользователь не найден\n";
    return false;
}
