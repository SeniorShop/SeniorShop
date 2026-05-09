#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>

struct User {
    std::string username;
    std::string password;
    std::string status;

    User();
    User(const std::string& name, const std::string& pass, const std::string& st);

    std::string serialize() const;

    static User deserialize(const std::string& line);

    bool operator==(const User& other) const;
};

#endif
