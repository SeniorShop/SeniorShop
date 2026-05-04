#include "SaleControl.hpp"
#include <iostream>
#include <chrono>

SaleController::SaleController(Storage& storage) : service(storage) 
{

}

void SaleController::start() 
{
    service.load();

    while (true) 
    {
        ui.showMenu();
        std::string cmd = ui.input();

        if (cmd == "exit") 
        {
            cart.clear();
            std::cout << "Сброс\n";
            continue;
        }

        if (cmd == "end") 
        {
            payment();
            return;
        }

        int id = std::stoi(cmd);
        int qty = ui.inputInt("Количество: ");

        if (!service.addToCart(cart, id, qty)) 
        {
            std::cout << "Ошибка: нет товара или недостаточно\n";
        }
    }
}

void SaleController::payment() 
{
    double total = cart.total();

    std::cout << "Итого: " << total << "\n";
    std::cout << "1 - карта\n2 - наличка\n";

    int type;
    std::cin >> type;

    if (type == 1) 
    {
        std::cout << "Оплата картой...\n";
        std::cout << "Оплачено\n";
    }
    else 
    {
        double money;
        std::cout << "Внесено: ";
        std::cin >> money;

        if (money < total) 
        {
            std::cout << "Недостаточно денег\n";
            payment();
            return;
        }

        std::cout << "Сдача: " << (money - total) << "\n";
    }

    service.applySale(cart);
}