#ifndef AUTH_SYSTEM_USER_HPP
#define AUTH_SYSTEM_USER_HPP
#include "Logger.h"
#include "User.h"
#include "Captcha.h"
#include "Storage.h"
#include <iostream>
#include <unordered_set>
#include <filesystem>

class AuthSystemUser {
    Storage start_storage;
    Captcha check_bot;
    std::vector<User> users;
    std::ofstream write_to;
    std::ifstream read_from;
    bool is_valid_username(const std::string& username) const;
    bool is_valid_pass(const std::string& password) const;
    bool is_valid_current_status(const std::string& current_status) const;
    bool is_super_admin = 0;
    bool user_exists(const std::string& username) const;
    void user_pass_change();
    void user_status_change();
    void load_from_file();
    void save_to_file(User& add_user);
public:
    AuthSystemUser();
    void register_user();
    void login();
    void change_user();
    void show_all_users();
    bool is_super_admin_exists();
    void remove_user();
};

#endif 
