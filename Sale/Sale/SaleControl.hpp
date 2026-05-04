#pragma once
#include "SaleService.hpp"
#include "SaleUI.hpp"

class SaleController 
{
public:
    SaleController(Storage& storage);

    void start();
    void payment();

private:
    SaleService service;
    SaleUI ui;
    Cart cart;
};
