#include "User.h"
#include <iostream>
#include <memory>
#include <sstream>

SuperAdminUser::SuperAdminUser()
    : User("SuperAdmin", "admin***123", "superadmin") {
}

std::unique_ptr<User> SuperAdminUser::clone() const {
    return std::make_unique<SuperAdminUser>(*this);
}

AdminUser::AdminUser(const std::string& name, const std::string& pass)
    : User(name, pass, "admin") {
}

std::unique_ptr<User> AdminUser::clone() const {
    return std::make_unique<AdminUser>(*this);
}

RegularUser::RegularUser(const std::string& name, const std::string& pass)
    : User(name, pass, "user") {
}

std::unique_ptr<User> RegularUser::clone() const {
    return std::make_unique<RegularUser>(*this);
}

std::unique_ptr<User> User::deserialize(const std::string& line) {
    std::istringstream ss(line);
    std::string name, pass, currentSt;
    if (std::getline(ss, name, '|') &&
        std::getline(ss, pass, '|') &&
        std::getline(ss, currentSt)) {
        if (currentSt == "superadmin")
            return std::make_unique<SuperAdminUser>();
        else if (currentSt == "admin")
            return std::make_unique<AdminUser>(name, pass);
        else if (currentSt == "user")
            return std::make_unique<RegularUser>(name, pass);
    }
    return nullptr;
}