#ifndef SALECONTROL_H
#define SALECONTROL_H

#include "SaleService.h"
#include "Cart.h"

class SaleController {
public:
    void start();
    void payment();
private:
    SaleService service;
    Cart cart;
};

#endif
