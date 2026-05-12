#include "User.h"

User::User() = default;
User::User(const std::string& name, const std::string& pass, const std::string& st)
    : username(name), password(pass), status(st) {}

std::string User::serialize() const {
    return username + "|" + password + "|" + status;
}

User User::deserialize(const std::string& line) {
    std::stringstream ss(line);
    User u;
    std::getline(ss, u.username, '|');
    std::getline(ss, u.password, '|');
    std::getline(ss, u.status);
    return u;
}

bool User::operator==(const User& other) const {
    return username == other.username;
}
