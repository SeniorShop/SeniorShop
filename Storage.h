#ifndef STORAGE_H
#define STORAGE_H

#include "Product.h"
#include "Supply.h"
#include "SupplyManager.h"
#include <vector>
#include <string>
#include <map>
#include <chrono>

class Storage {
    std::vector<Product> goods;
    std::string current_user_role = "user";
    SupplyManager supply_manager;

public:
    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename);
    void add_product();
    void show_all();
    void show_valid_products();
    void check_expired_products();
    void start();
    void set_current_user_role(const std::string& role);
    void add_supply_products(const Supply& supply);

private:
    bool check_characteristics(const Product& product);
    bool is_date_range_valid(const std::string& begin, const std::string& end);
    bool validate_dates(const std::string& begin, const std::string& end);
    bool is_expired(const std::string& date_str);
    std::chrono::sys_days parse_date(const std::string& date_str);
    std::string format_field(std::string str, std::size_t width) const;

    void list_superadmin_actions();
    void list_admin_actions();
    void list_user_actions();

    void supply_menu();
};

#endif
