#include "AuthSystemUser.h"
#include <filesystem>
#include <algorithm>
#include <unordered_set>


AuthSystemUser::AuthSystemUser() {
    load_from_file();
    //Если в файле нет суперадмина, то создаём его с фиксированными данными !! SuperAdmin", "admin***123", "superadmin
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

// Загрузка из файла 
void AuthSystemUser::load_from_file() {
    users.clear();
    std::ifstream in_file("Users.txt");
    if (!in_file.is_open()) {
        std::cerr << "Ошибка открытия файла Users.txt" << std::endl;
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
        std::cerr << "Ошибка открытия файла Users.txt для записи" << std::endl;
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

    // Спецсимволы
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
    std::cout << "Введите логин: ";
    Getline(username);
    std::cout << "Введите пароль: ";
    Getline(password);

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
    return nullptr;
}


void AuthSystemUser::register_user() {
    
    if (!currentUser_ || currentUser_->status != "superadmin") {
        std::cerr << "Только суперадминистратор может создавать новых пользователей" << std::endl;
        return;
    }

    std::string username, password;
    std::cout << "Введите логин нового пользователя: ";
    Getline(username);

    if (!is_valid_username(username)) {
        std::cerr << "Логин должен быть от 5 до 20 символов, только буквы и цифры" << std::endl;
        return;
    }

    if (user_exists(username)) {
        std::cerr << "Пользователь с таким логином уже существует" << std::endl;
        return;
    }

    std::cout << "Введите пароль: ";
    Getline(password);

    if (!is_valid_pass(password)) {
        std::cerr << "Пароль должен быть не менее 8 символов и содержать минимум 3 спецсимвола" << std::endl;
        return;
    }

    std::cout << "Выберите статус:\n1 - Обычный пользователь\n2 - Администратор\nВыбор: ";
    std::string choose;
    Getline(choose);

    if (choose != "1" && choose != "2") // добавить выход exit
    {
        std::cerr << "Ошибка! Неверный выбор\n";
        return;
    }

 
    std::unique_ptr<User> newUser;
    if (choose == "1")
    {
        newUser = std::make_unique<RegularUser>(username, password);
       
    }
    else if (choose == "2")
    {
        newUser = std::make_unique<AdminUser>(username, password);
    }



    users.push_back(std::move(newUser));
    save_to_file();
    Logger::log_attempt(username, true);
    std::cout << "Пользователь успешно зарегистрирован." << std::endl;
}


void AuthSystemUser::show_all_users() {
    std::cout << "Список пользователей:" << std::endl;
    for (const auto& u : users) {
        std::cout << "Логин: " << u->username
            << ", Статус: " << u->status << std::endl;
    }
}


void AuthSystemUser::change_user() {
    if (!currentUser_ || currentUser_->status != "superadmin") {
        std::cerr << "Только superadmin может изменять пользователей" << std::endl;
        return;
    }

    std::cout << "1. Изменить пароль\n2. Изменить статус\nВыбор: ";
    std::string choose;
    Getline(choose);
  
    if (choose == "1") {
        user_pass_change();
    }
    else if (choose == "2") {
        user_status_change();
    }
    else {
        std::cerr << "Ошибка! Неверный выбор" << std::endl;
        return;
    }
}

void AuthSystemUser::user_pass_change() {
  
    std::string name, password , status, new_password;
    std::cout << "Введите логин пользователя: ";
    Getline(name);
    std::cout << "Введите текущий пароль: ";
    Getline(password);
    std::cout << "Введите текущий статус: ";
     Getline(status);
    
    for (auto& u : users) {
        if (u->username == name && u->password == password && u->status == status) {
            std::cout << "Введите новый пароль: ";
            std::cin >> new_password;
            if (!is_valid_pass(new_password)) {
                std::cerr << "Новый пароль не соответствует требованиям" << std::endl;
                return;
            }
            u->password = new_password;
            save_to_file();
            std::cout << "Пароль успешно изменён." << std::endl;
            return;
        }
    }
    std::cerr << "Пользователь не найден или данные не совпадают" << std::endl;
}

void AuthSystemUser::user_status_change() {
    std::string name, password, status, new_status;
    std::cout << "Введите логин пользователя: ";
    Getline(name);
    std::cout << "Введите пароль: ";
    Getline(password);
    std::cout << "Введите текущий статус: ";
    Getline(status);

    for (auto& u : users) {
        if (u->username == name && u->password == password && u->status == status) {
            
            if (u->status == "superadmin") {
                std::cerr << "Нельзя изменить статус суперадминистратора" << std::endl;
                return;
            }
            std::cout << "Введите новый статус (admin/user): ";
            std::cin >> new_status;
            if (new_status != "admin" && new_status != "user") {
                std::cerr << "Недопустимый статус" << std::endl;
                return;
            }
            u->status = new_status;
            save_to_file();
            std::cout << "Статус успешно изменён." << std::endl;
            return;
        }
    }
    std::cerr << "Пользователь не найден или данные не совпадают" << std::endl;
}


void AuthSystemUser::remove_user() {
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

    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->username == name && (*it)->password == password && (*it)->status == status) {
            if ((*it)->status == "superadmin") {
                std::cerr << "Нельзя удалить суперадминистратора" << std::endl;
                return;
            }
            users.erase(it);
            save_to_file();
            std::cout << "Пользователь успешно удалён." << std::endl;
            return;
        }
    }
    std::cerr << "Пользователь не найден или данные не совпадают" << std::endl;
}