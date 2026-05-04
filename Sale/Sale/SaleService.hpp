#pragma once
#include "Storage.hpp"
#include "Cart.hpp"
#include <vector>

class SaleService {
private:
    Storage& storage;
    std::vector<Product> products;

public:
    SaleService(Storage& s);

    void load();
    const std::vector<Product>& getProducts() const;

    bool addToCart(Cart& cart, int id, int qty);
    void applySale(const Cart& cart);
};