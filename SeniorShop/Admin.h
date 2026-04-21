#ifndef ADMIN_H
#define ADMIN_H
#include "User.h"

struct Admin final : public User {
    std::size_t moderation_level;
    void set_name(const std::string& set_name) override;
    void set_password(const std::string& set_password) override;
    void set_status(const std::string& set_status) override;
    void set_moderation_level(const std::size_t& set_moderation_level);
    std::string get_name() const override;
    std::string get_password() const override;
    std::string get_status() const override;
    std::size_t get_moderation_level() const;
};

#endif // ADMIN_H
