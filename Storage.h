#ifndef STORAGE_H
#define STORAGE_H
#include "Product.h"
#include "Supply.h"
#include "SupplyManager.h"
#include <vector>
#include <string>
#include <chrono>

class AuthSystemUser;

class Storage {
    std::vector<Product> goods;
    SupplyManager supply_manager;
    Storage& get_storage() { return *this; }
public:
    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename);
    void add_product();
    void show_all();
    void show_valid_products();
    void check_expired_products();

    void start(const std::string& userStatus, AuthSystemUser& authSystem);
    void add_supply_products(const Supply& supply);
    SupplyManager& get_supply_manager() { return supply_manager; }

private:
    bool check_characteristics(const Product& product);
    bool is_date_range_valid(const std::string& begin, const std::string& end);
    bool validate_dates(const std::string& begin, const std::string& end);
    bool is_expired(const std::string& date_str);
    std::chrono::sys_days parse_date(const std::string& date_str);
    std::string format_field(std::string str, std::size_t width) const;

    void super_admin_menu(AuthSystemUser& authSystem);
    void admin_menu();
    void user_menu();
    void supply_menu();
};

#endif
