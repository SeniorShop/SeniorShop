#ifndef SALESERVICE_H
#define SALESERVICE_H

#include "../include/Cart.h"
#include "../include/Product.h"
#include <vector>
#include <chrono>

class SaleService {
    std::vector<Product> products;
public:
    std::vector<Product> load();
    void refresh();
    const std::vector<Product>& get_products() const;
    bool add_to_cart(Cart& cart, unsigned int id, unsigned int count);
    void show_cart(const Cart& cart);
    void apply_sale(const Cart& cart, double final_total);
    bool is_expired(const std::string& date_str);
    std::chrono::sys_days parse_date(const std::string& date_str);
};

#endif
