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

class Storage {
    std::vector<Product> goods;
public:
    void load_from_file(const std::string& product_database);
    void save_to_file(const std::string& product_database);
    void add_product();
    void show_all();
    void start();
    void check_expired_products();
    void show_valid_products();
private:
    bool check_characteristics(const Product& analyse_product);
    bool is_date_range_valid(const std::string& begin_date_str, const std::string& end_date_str);
    bool validate_dates(const std::string& begin_date, const std::string& end_date);
    bool is_expired(const std::string& end_date_str);
    std::chrono::sys_days parse_date(const std::string& date_str);
    std::string format_field(std::string str, std::size_t width) const;
};

#endif // STORAGE_H
