#include "SaleService.h"
#include "Check.h"
#include <ctime>
#include <thread>
#include <iostream>
#include <fstream>

std::vector<Product> SaleService::load() {
    std::vector<Product> products;
    std::ifstream file("Product.txt");

    Product p;
    unsigned int index = 1;
    while (file >> p.name >> p.category >> p.price
           >> p.article >> p.begin_date >> p.end_date
           >> p.count >> p.manufacturer >> p.country) {
        if (p.count > 0) {
            p.id = index++;
            products.push_back(p);
        }
    }
    file.close();

    return products;
}

bool SaleService::add_to_cart(Cart& cart, unsigned int id, unsigned int count) {
    std::vector<Product> products = load();

    if (id < 1 || id > products.size()) {
        std::cout << "Ошибка: неверный ID товара\n";
        return false;
    }

    const Product& p = products[id - 1];

    if (count == 0) {
        std::cerr << "Ошибка: количество должно быть больше 0\n";
        return false;
    }

    if (p.count < count) {
        std::cerr << "Ошибка: на складе только " << p.count << " шт. товара\n";
        return false;
    }

    cart.add(p, count);
    return true;
}

void SaleService::show_сart(const Cart& cart) {
    std::cout << "\n\n\n\t\t\tКОРЗИНА\n\n\n";
    for (const auto& item : cart.get_items()) {
        std::cout << item.product.name << " * " << item.count << " = "
                  << item.product.price * item.count << " руб.\n";
    }
    std::cout << "Итого: " << cart.total() << " руб.\n";
}

void SaleService::apply_sale(const Cart& cart) {

    for (const auto& item : cart.get_items()) {
        for (auto& p : products) {
            if (p.article == item.product.article) {
                p.count -= item.count;
                break;
            }
        }
    }

    std::ofstream file("Product.txt");
    for (const auto& p : products) {
        file << p.name << " " << p.category << " " << p.price << " "
             << p.article << " " << p.begin_date << " " << p.end_date << " "
             << p.count << " " << p.manufacturer << " " << p.country << "\n";
    }
    file.close();

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::string date_str = std::ctime(&now_time);
    date_str.pop_back();

    for (const auto& item : cart.get_items()) {
        Check check(TransactionType::Sale, item.product.name, item.product.price,
                    item.count, "Сотрудник", date_str);
        check.save_check("checks.txt");
    }

    std::cout << "Продажа записана в отчёт\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

