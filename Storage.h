#ifndef STORAGE_H
#define STORAGE_H
#include "Product.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <unordered_set>
#include <map>

class AuthSystemUser;

class Storage {
    std::vector<Product> goods;
    AuthSystemUser* authSystem = nullptr;
public:
    void setAuthSystem(AuthSystemUser* auth);
    void load_from_file(const std::string& product_database);
    void save_to_file(const std::string& product_database);
    void add_product();
    void show_all();                               
    void show_all_from_file(const std::string& filename); 
    void start(const std::string& userStatus);
    void check_expired_products();
    void show_valid_products();
    void super_admin_menu();
    void admin_menu();
    void user_menu();


    //void remove_product();          // списание товара
    //void change_price();            // изменение цены
    //void edit_storage_menu();       // подменю редактирования склада
    //void report();                  // отчёт
    //void supplies();                

private:
    
    bool is_date_range_valid(const std::string& begin_date_str, const std::string& end_date_str);
    bool validate_dates(const std::string& begin_date, const std::string& end_date);
    bool is_expired(const std::string& end_date_str);
    std::chrono::sys_days parse_date(const std::string& date_str);
    std::string format_field(std::string str, std::size_t width) const;
};

#endif