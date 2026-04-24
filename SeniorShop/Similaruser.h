#ifndef SIMILARUSER_H
#define SIMILARUSER_H
#include "User.h"

struct SimilarUser final : public User {
    void set_name(const std::string& set_name) override;
    void set_password(const std::string& set_password) override;
    void set_status(const std::string& set_status) override;
    std::string get_name() const override;
    std::string get_password() const override;
    std::string get_status() const override;
};

#endif // SIMILARUSER_H
