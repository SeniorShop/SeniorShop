#pragma once
#include <vector>
#include "Product.hpp"

struct CartItem 
{
    Product product;
    int quantity;
};

class Cart 
{
private:
    std::vector<CartItem> items;

public:
    void add(const Product& product, int qty);
    double total() const;
    bool empty() const;
    void clear();
    const std::vector<CartItem>& getItems() const;
};