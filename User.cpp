#include "User.h"

User::~User() = default;

void User::set_name(const std::string& set_name) {
    username = set_name;
}
void User::set_password(const std::string& set_password) {
    password = set_password;
}
void User::set_status(const std::string& set_status) {
    status = set_status;
}
std::string User::get_name() const {
    return username;
}
std::string User::get_password() const {
    return password;
}
std::string User::get_status() const {
    return status;
}
