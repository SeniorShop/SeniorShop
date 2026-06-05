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
    Storage start_storage;
    Captcha check_bot;
    std::vector<std::unique_ptr<User>> users;
    User* currentUser_ = nullptr;

    bool is_valid_username(const std::string& username) const;
    bool is_valid_pass(const std::string& password) const;
    bool user_exists(const std::string& username) const;
    void load_from_file();
    void save_to_file();
    void user_pass_change();
    void user_status_change();

public:
    AuthSystemUser();

    User* login();
    void register_user();
    void change_user();
    void show_all_users();
    void remove_user();

    User* getCurrentUser() const { return currentUser_; }

    const std::vector<std::unique_ptr<User>>& get_users() const { return users; }
    std::string get_all_users_JSON() const;
    bool register_user_direct(const std::string& username, const std::string& password, const std::string& role);
    bool delete_user_direct(const std::string& username);
    bool is_super_admin() const { return currentUser_ && currentUser_->status == "superadmin"; }
};

#endif
