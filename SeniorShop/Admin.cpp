#include "Admin.h"

void Admin::set_name(const std::string &set_name) {
    username = set_name;
}
void Admin::set_password(const std::string &set_password) {
    password = set_password;
}
void Admin::set_status(const std::string &set_status) {
    status = set_status;
}
std::string Admin::get_name() const {
    return username;
}
std::string Admin::get_password() const {
    return password;
}
std::string Admin::get_status() const {
    return status;
}
void Admin::set_moderation_level(const std::size_t &set_moderation_level) {
    moderation_level = set_moderation_level;
}
std::size_t Admin::get_moderation_level() const {
    return moderation_level;
}
