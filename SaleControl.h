#pragma once
#include "SaleService.h"
#include "Cart.h"
#include <vector>

class SaleController {
public:
    void start();
    void payment();
private:
    SaleService service;
    Cart cart;
};
