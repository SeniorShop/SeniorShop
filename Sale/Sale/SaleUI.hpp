#pragma once
#include "Cart.hpp"

class SaleUI 
{
public:
    void showMenu();
    void showCart(const Cart& cart);

    std::string input();
    int inputInt(const std::string& msg);
};
