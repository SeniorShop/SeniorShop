#pragma once
#include "SaleService.h"
#include <vector>

class SaleController {
    SaleService show;
    std::vector<Product> list_products;
public:
    void start();
    void payment();
private:
    SaleService service;
    Cart cart;
};
