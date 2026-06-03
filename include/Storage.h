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

    // Получение данных
    const std::vector<Product>& get_all_products() const { return goods; }
    const Product* get_product_by_article(int article) const;

    // CRUD операции
    bool add_product_from_JNI(const Product& product);
    bool delete_product_by_article(int article);
    bool update_product_count(int article, unsigned int new_count);
    bool update_product_price(int article, double new_price);
    bool update_product_name(int article, const std::string& new_name);

    // Продажи
    bool add_to_cart(int article, unsigned int count);
    double get_cart_total() const;
    void clear_cart();
    bool apply_sale_from_cart(const std::vector<std::pair<int, unsigned int>>& items,
                              double final_total, const std::string& employee);

    // Проверки
    bool is_product_expired(const std::string& end_date_str) const;
    bool is_expired(const std::string& end_date_str) const;
    std::chrono::sys_days parse_date(const std::string& date_str) const;

private:
    bool check_characteristics(const Product& analyse_product);
    bool is_date_range_valid(const std::string& begin_date_str, const std::string& end_date_str) const;
    bool validate_dates(const std::string& begin_date, const std::string& end_date) const;
    std::string format_field(std::string str, std::size_t width) const;
    std::string date_to_string(std::chrono::sys_days tp) const;
};

#endif
