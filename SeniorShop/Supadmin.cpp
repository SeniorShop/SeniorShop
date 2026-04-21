#include "Supadmin.h"

void Supadmin::set_name(const std::string &set_name) {
    username = set_name;
}
void Supadmin::set_password(const std::string &set_password) {
    password = set_password;
}
void Supadmin::set_status(const std::string &set_status) {
    status = set_status;
}
void Supadmin::set_privileges() {privileges = true;}
std::string Supadmin::get_name() const {
    return username;
}
std::string Supadmin::get_password() const {
    return password;
}
std::string Supadmin::get_status() const {
    return status;
}
bool Supadmin::get_privileges() const {return privileges;}

