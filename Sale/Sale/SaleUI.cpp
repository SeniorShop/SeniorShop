#include "SaleUI.hpp"
#include <iostream>

void SaleUI::showMenu() 
{
    std::cout << "\nВведите ID товара или команды:\n";
    std::cout << "end - оплата\n";
    std::cout << "exit - сброс\n";
}

void SaleUI::showCart(const Cart& cart) 
{
    std::cout << "\nКорзина:\n";
    for (auto& item : cart.getItems()) 
    {
        std::cout << item.product.GetName()
            << " x" << item.quantity << "\n";
    }
    std::cout << "Сумма: " << cart.total() << "\n";
}

std::string SaleUI::input() 
{
    std::string s;
    std::cin >> s;
    return s;
}

int SaleUI::inputInt(const std::string& msg) 
{
    int v;
    std::cout << msg;
    std::cin >> v;
    return v;
}