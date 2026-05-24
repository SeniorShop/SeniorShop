#ifndef STORAGE_H
#define STORAGE_H
#include "Product.h"
#include "Supply.h"
#include "SupplyManager.h"
#include "SaleControl.h"
#include "Check.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <unordered_set>
#include <map>

class AuthSystemUser;

class Storage {
    SaleController start_sales;
    SupplyManager supply_manager;
    std::vector<Product> goods;
    AuthSystemUser* authSystem = nullptr;
    Check get_check;
public:
    void set_auth_system(AuthSystemUser* auth);
    void load_from_file(const std::string& product_database);
    void save_to_file(const std::string& product_database);
    void add_product();
    void show_all();
    void add_supply_products(const Supply& supply);
    void start(const std::string& user_status);
    void check_expired_products();
    void show_valid_products();
    void super_admin_menu();
    void admin_menu();
    void user_menu();
    void supply_menu();
    void change_product_price();
    void show_all_for_actions();

    void edit_storage_menu(std::string status = "superadmin");
    void delete_product();
    void change_product_name();
    void replenish_product();
    void auto_writeoff_expired();
    void writeoff_product();

private:
    bool check_characteristics(const Product& analyse_product);
    bool is_date_range_valid(const std::string& begin_date_str, const std::string& end_date_str);
    bool validate_dates(const std::string& begin_date, const std::string& end_date);
    bool is_expired(const std::string& end_date_str);
    std::chrono::sys_days parse_date(const std::string& date_str);
    std::string format_field(std::string str, std::size_t width) const;
    std::string date_to_string(std::chrono::sys_days tp) const;
};

#endif
