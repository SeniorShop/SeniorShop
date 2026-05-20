#include "SaleService.h"
#include "Check.h"
#include "Storage.h"
#include <ctime>
#include <thread>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

std::vector<Product> SaleService::load() {
    products.clear();
    std::ifstream file("Product.txt");
    if (!file.is_open()) {
        return products;
    }

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

void SaleService::refresh() {
    products.clear();
    std::ifstream file("Product.txt");
    if (!file.is_open()) {
        return;
    }

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
}

const std::vector<Product>& SaleService::get_products() const {
    return products;
}

bool SaleService::add_to_cart(Cart& cart, unsigned int id, unsigned int count) {
    refresh();
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

void SaleService::apply_sale(const Cart& cart, double final_total) {
    refresh();

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
    std::tm* tm = std::localtime(&now_time);

    std::stringstream ss;
    ss << std::setfill('0');
    ss << std::setw(2) << tm->tm_mday << "."
       << std::setw(2) << (tm->tm_mon + 1) << "."
       << std::setw(4) << (tm->tm_year + 1900) << " "
       << std::setw(2) << tm->tm_hour << ":"
       << std::setw(2) << tm->tm_min << ":"
       << std::setw(2) << tm->tm_sec;
    std::string date_str = ss.str();

    for (const auto& item : cart.get_items()) {
        double original_item_price = item.product.price * item.count;
        double total_original = cart.total();
        double ratio = original_item_price / total_original;
        double discounted_price = final_total * ratio / item.count;

        Check check(TransactionType::Sale, item.product.name, discounted_price,
                    item.count, "Сотрудник", date_str);
        check.save_check("Checks.txt");
    }

    std::cout << "Продажа записана в отчёт\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
