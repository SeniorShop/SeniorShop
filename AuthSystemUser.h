#ifndef AUTH_SYSTEM_USER_HPP
#define AUTH_SYSTEM_USER_HPP
#include "User.h"
#include "Logger.h"
#include "Captcha.h"
#include "Storage.h"
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

class AuthSystemUser {
    Storage start_storage;                      //экземпляр склада (используется только для запуска)
    Captcha check_bot;                          //капча для входа
    std::vector<std::unique_ptr<User>> users;   // полиморфное хранилище
    User* currentUser_ = nullptr;               // вошедший пользователь

    // Вспомогательные методы
    bool is_valid_username(const std::string& username) const;
    bool is_valid_pass(const std::string& password) const;
    bool user_exists(const std::string& username) const;

    void load_from_file();
    void save_to_file();

    // Внутренние методы редактирования (вызываются из change_user)
    void user_pass_change();
    void user_status_change();

public:
    AuthSystemUser();

    User* login();              // возвращает указатель на вошедшего пользователя (nullptr при ошибке)
    void register_user();       // создание нового пользователя  !! superadmin
    void change_user();         // изменение существующего пользователя !! superadmin
    void show_all_users();      // просмотр всех пользователей // superadmin или admin
    void remove_user();         // удаление пользователя !! superadmin

    User* getCurrentUser() const { return currentUser_; }
};

#endif