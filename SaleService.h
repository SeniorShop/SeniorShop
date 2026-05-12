#pragma once
#include "Cart.h"
#include <vector>

class SaleService {
    std::vector<Product> products;
public:
    std::vector<Product> load();
    const std::vector<Product>& get_products() const;
    bool add_to_cart(Cart& cart, unsigned int id, unsigned int count);
    void show_сart(const Cart& cart);
    void apply_sale(const Cart& cart);
};
