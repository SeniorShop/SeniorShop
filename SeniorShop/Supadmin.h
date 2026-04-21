#ifndef SUPADMIN_H
#define SUPADMIN_H
#include "User.h"
struct Supadmin final : public User {
    bool privileges = 0;
    void set_name(const std::string& set_name) override;
    void set_password(const std::string& set_password) override;
    void set_status(const std::string& set_status) override;
    void set_privileges();
    std::string get_name() const override;
    std::string get_password() const override;
    std::string get_status() const override;
    bool get_privileges() const;
};

#endif // SUPADMIN_H
