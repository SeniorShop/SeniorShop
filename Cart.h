#ifndef CART_H
#define CART_H
#include "Product.h"
#include <vector>

struct CartItem  {
    Product product;
    unsigned int count;
};

class Cart {
    std::vector<CartItem> items;
public:
    void add(const Product& product, unsigned int count);
    double total() const;
    bool empty() const;
    void clear();
    const std::vector<CartItem>& get_items() const;
};

#endif
