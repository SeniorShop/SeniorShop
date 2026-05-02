#ifndef PRODUCT_H
#define PRODUCT_H
#include <iostream>
#include <string>

inline void Getline(std::string& str) {
    std::getline(std::cin, str, '\n');
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(365000, '\n');
    }
}

struct Product {
    std::string name;
    std::string category;
    double price;
    int article;
    std::string begin_date;
    std::string end_date;
    unsigned int count;
    std::string manufacturer;
    std::string country;
    Product(const Product& other);
    Product();
    bool operator==(const Product& other) const;
};

#endif // !PRODUCT_H
