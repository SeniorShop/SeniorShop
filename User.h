#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>

struct User {
    virtual ~User();
    std::string username;
    std::string password;
    std::string status;
    virtual void set_name(const std::string& set_name);
    virtual void set_password(const std::string& set_password);
    virtual void set_status(const std::string& set_status);
    virtual std::string get_name() const;
    virtual std::string get_password() const;
    virtual std::string get_status() const;
};

#endif // USER_H
