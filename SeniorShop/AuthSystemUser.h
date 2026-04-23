#ifndef AUTH_SYSTEM_USER_HPP
#define AUTH_SYSTEM_USER_HPP
#include "Logger.h"
#include "User.h"

class AuthSystemUser {
    std::ofstream write_to;
    std::ifstream read_from;
    std::vector<User> users;
    std::string users_filename;
    bool is_valid_username(const std::string& username) const;
    bool is_valid_pass(const std::string& password) const;
    bool is_valid_current_status(const std::string& current_status) const;
    bool user_exists(const std::string& username) const;
    void load_from_file();
    void save_to_file(User& add_user);
public:
    AuthSystemUser();
    bool register_user();
    bool login();
    bool change_user();
    void show_all_users();
    bool is_super_admin_exists();
};

#endif 
