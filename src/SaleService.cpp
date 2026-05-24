#include "../include/SaleService.h"
#include "../include/Check.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>
#ifdef _WIN32
#include "Windows.h"
#else
#include <stdlib.h>
#endif

std::vector<Product> SaleService::load() {
    products.clear();
    std::ifstream file("Product.txt");
    if (!file.is_open()) return products;

    Product p;
    unsigned int index = 1;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::getline(ss, p.name, '|');
        std::getline(ss, p.category, '|');
        ss >> p.price; ss.ignore();
        ss >> p.article; ss.ignore();
        std::getline(ss, p.begin_date, '|');
        std::getline(ss, p.end_date, '|');
        ss >> p.count; ss.ignore();
        std::getline(ss, p.manufacturer, '|');
        std::getline(ss, p.supplier, '|');
        std::getline(ss, p.country);

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
    if (!file.is_open()) return;

    Product p;
    unsigned int index = 1;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::getline(ss, p.name, '|');
        std::getline(ss, p.category, '|');
        ss >> p.price; ss.ignore();
        ss >> p.article; ss.ignore();
        std::getline(ss, p.begin_date, '|');
        std::getline(ss, p.end_date, '|');
        ss >> p.count; ss.ignore();
        std::getline(ss, p.manufacturer, '|');
        std::getline(ss, p.supplier, '|');
        std::getline(ss, p.country);

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

    if (is_expired(p.end_date)) {
        std::cerr << "Ошибка: товар просрочен! Нельзя добавить в корзину.\n";
        return false;
    }

    auto end_date = parse_date(p.end_date);
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    auto days_left = std::chrono::duration_cast<std::chrono::days>(end_date - today).count();
    if (days_left <= 7 && days_left > 0) {
        std::cout << "Внимание! Срок годности товара \"" << p.name
                  << "\" истекает через " << days_left << " дней!\n";
    }

    cart.add(p, count);
    return true;
}

void SaleService::show_cart(const Cart& cart) {
    if (cart.empty()) {
        std::cout << "Корзина пуста.\n";
        return;
    }
    std::cout << "\n\n\n\t\t\tКОРЗИНА\n\n\n";
    for (const auto& item : cart.get_items()) {
        std::cout << item.product.name << " x " << item.count
            << " = " << item.product.price * item.count << " руб.\n";
    }
    std::cout << "ИТОГО: " << cart.total() << " руб.\n";
}

void SaleService::apply_sale(const Cart& cart, double final_total) {
    if (cart.empty()) return;
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
        file << p.name << "|"
             << p.category << "|"
             << p.price << "|"
             << p.article << "|"
             << p.begin_date << "|"
             << p.end_date << "|"
             << p.count << "|"
             << p.manufacturer << "|"
             << p.supplier << "|"
             << p.country << "\n";
    }
    file.close();

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&t);
    std::stringstream ss;
    ss << std::setfill('0')
       << std::setw(2) << tm->tm_mday << "."
       << std::setw(2) << (tm->tm_mon + 1) << "."
       << (tm->tm_year + 1900) << " "
       << std::setw(2) << tm->tm_hour << ":"
       << std::setw(2) << tm->tm_min << ":"
       << std::setw(2) << tm->tm_sec;
    std::string date_str = ss.str();

    Check check(TransactionType::Sale, final_total, "Сотрудник", date_str);
    check.save_check("Checks.txt");

    std::cout << "Продажа оформлена. Сумма: " << final_total << " руб.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool SaleService::is_expired(const std::string& date_str) {
    try {
        auto end_date = parse_date(date_str);
        auto now = std::chrono::system_clock::now();
        auto today = std::chrono::floor<std::chrono::days>(now);
        return end_date < today;
    } catch (...) {
        return true;
    }
}

std::chrono::sys_days SaleService::parse_date(const std::string& date_str) {
    int day, month, year;
    char dot1, dot2;
    std::stringstream ss(date_str);
    ss >> day >> dot1 >> month >> dot2 >> year;
    if (year < 100) year += 2000;
    return std::chrono::sys_days{
        std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day)
    };
}
