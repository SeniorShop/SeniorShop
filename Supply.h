#ifndef SUPPLY_H
#define SUPPLY_H
#include <string>
#include <chrono>
#include "Product.h"

struct Supply {
    unsigned int number_supply;
    std::string name_user;
    std::chrono::sys_days date;
    std::chrono::sys_days date_acception;
    std::chrono::sys_days date_processing;
    std::string responsible_person;
    Product product_name;
    bool is_actually;
    std::string status;

    Supply();

    static std::string date_to_string(std::chrono::sys_days tp);
    static std::chrono::sys_days string_to_date(const std::string& str);

    void print() const;
};

#endif
