#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>

struct User {
    std::string username;
    std::string password;
    std::string status;
    void set_name(const std::string& set_name);
    void set_password(const std::string& set_password);
    void set_status(const std::string& set_status);
    std::string get_name() const;
    std::string get_password() const;
    std::string get_status() const;
    bool operator==(const User& other) const;
};

#endif // USER_H
